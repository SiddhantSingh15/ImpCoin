#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <nng/nng.h>
#include <nng/protocol/bus0/bus.h>
#include <nng/supplemental/util/platform.h>
#include <binn.h>
#include <sodium.h>

#include "lib/linked_list.h"
#include "lib/transaction.h"
#include "lib/block.h"
#include "lib/blockchain.h"
#include "lib/utils.h"
#include "lib/messages.h"

// We only dereference a bc_ptr when the lock is acquired
pthread_mutex_t lock;

typedef enum { IDLE, INIT, RECV, WAIT, SEND } state;

typedef struct {
  state state;
  nng_aio *aio;
  nng_msg *msg;
  nng_socket sock;
  blockchain **bc_ptr;
  const char *username;
} worker_t;

void fatal(const char *func, int rv) {
  fprintf(stderr, "%s: %s\n", func, nng_strerror(rv));
  exit(1);
}

void send_mine_message(blockchain *bc, worker_t *w) {
  int rv;
  nng_msg *nng_msg;
  blockchain_msg *bc_msg = malloc(sizeof(blockchain_msg));
  binn *obj;

  char *temp = to_hex_string_hash(bc->latest_block->hash);
  free(temp);

  w->state = SEND;
  if ((rv = nng_msg_alloc(&nng_msg, 0)) != 0) {
    fatal("nng_msg_alloc", rv);
  }
  bc_msg->bc = bc;
  strcpy(bc_msg->username, w->username);
  strcpy(bc_msg->type, "mine");
  obj = serialize_bc_msg(bc_msg);
  free(bc_msg);
  nng_msg_insert(nng_msg, binn_ptr(obj), binn_size(obj));
  nng_aio_set_msg(w->aio, nng_msg);
  nng_send_aio(w->sock, w->aio);
  binn_free(obj);
}

void send_transaction_message(char *to, uint64_t amount, worker_t *w) {

  int rv;
  nng_msg *nng_msg;
  binn *obj;

  transaction_msg *t_msg = init_transaction_msg(amount, w->username, to);
  obj = serialize_t_msg(t_msg);

  free(t_msg);

  if ((rv = nng_msg_alloc(&nng_msg, 0)) != 0) {
    fatal("nng_msg_alloc", rv);
  }

  nng_msg_append(nng_msg, binn_ptr(obj), binn_size(obj));
  nng_aio_set_msg(w->aio, nng_msg);

  w->state = SEND;
  nng_send_aio(w->sock, w->aio);
  binn_free(obj);
}

void incoming_callback(void *arg) {
  worker_t *w = arg;
  int rv;
  binn *buffer;
  char type[MESSAGE_TYPE_SIZE];

  switch (w->state) {
  case INIT:
    w->state = RECV;
    nng_recv_aio(w->sock, w->aio);
    break;
  case RECV:
    if ((rv = nng_aio_result(w->aio)) != 0) {
      if (strcmp(nng_strerror(rv), "Object closed") != 0) {
        fatal("nng_recv_aio", rv);
      } else {
        break;
      }
    }
    w->state = RECV;
    w->msg = nng_aio_get_msg(w->aio);
    buffer = (binn *)nng_msg_body(w->msg);
    strcpy(type, binn_object_str(buffer, "type"));
    if (strcmp(type, "mine") == 0) {

      if (strcmp(w->username, binn_object_str(buffer, "username")) == 0) {
        // This was sent by us, ignore
        nng_recv_aio(w->sock, w->aio);
        break;
      }

      printf("I received a block from %s\n",
             binn_object_str(buffer, "username"));
      blockchain_msg *bc_msg = deserialize_bc_msg(buffer);

      // lock
      pthread_mutex_lock(&lock);

      if (blockchain_valid(*w->bc_ptr, bc_msg->bc)) {
        // blockchain *to_free = *w->bc_ptr;
        *w->bc_ptr = bc_msg->bc;
        // free_blockchain(to_free);
        // printf("%s\n", blockchain_to_string(*w->bc));
        // print_block((*(w->bc_ptr))->latest_block);
      } else {
        free_blockchain(bc_msg->bc);
      }
      free(bc_msg);
      // unlock
      pthread_mutex_unlock(&lock);

    } else if (strcmp(type, "trans") == 0) {
      transaction_msg *tc_msg = deserialize_t_msg(buffer);
      transaction *tc = to_transaction(tc_msg);
      free(tc_msg);

      // lock
      pthread_mutex_lock(&lock);
      if (is_valid_transaction(tc, *(w->bc_ptr))) {
        printf("New transaction received: ");
        print_transaction(tc);
        ll_append((*(w->bc_ptr))->mempool, tc);
        printf("Current state of mempool:\n");
        ll_print((*(w->bc_ptr))->mempool, to_string_transaction);
      } else {
        if (strcmp(w->username, binn_object_str(buffer, "username")) == 0) {
          printf("%sREJECTED. You do not have enough IMPs%s\n",
            BOLDRED, NOCOLOUR);
        }
        free_transaction(tc);
      }
      // unlock
      pthread_mutex_unlock(&lock);
    }
    nng_msg_free(w->msg);
    nng_recv_aio(w->sock, w->aio);
    break;
  default:
    break;
  }
}

void outgoing_callback(void *arg) {
  worker_t *w = arg;

  switch (w->state) {
  case IDLE:
    break;
  case SEND:
    w->state = IDLE;
    break;
  default:
    break;
  }
}

worker_t *alloc_worker(nng_socket sock, void (* callback)(void *)) {
  worker_t *w;
  int rv;

  if ((w = nng_alloc(sizeof(*w))) == NULL) {
    fatal("nng_alloc", NNG_ENOMEM);
  }

  if ((rv = nng_aio_alloc(&w->aio, callback, w)) != 0) {
    fatal("nng_aio_alloc", rv);
  }

  w->sock = sock;
  return w;
}

nng_socket start_node(const char *our_url, worker_t *incoming[],
                      worker_t *outgoing[], blockchain **bc_ptr,
                      const char *username) {
  nng_socket sock;
  int rv;
  int i;

  printf("Starting ImpCoin node...\n");

  /*  Create the socket. */
  rv = nng_bus0_open(&sock);
  if (rv != 0) {
    fatal("nng_bus0_open", rv);
  } else {
    printf("Created bus socket\n");
  }

  if ((rv = nng_listen(sock, our_url, NULL, 0)) != 0) {
    fatal("nng_listen", rv);
  } else {
    printf("Listening at %s\n", our_url);
  }

  for (i = 0; i < PARALLEL; i++) {

    incoming[i] = alloc_worker(sock, incoming_callback);
    incoming[i]->state = INIT;
    incoming[i]->bc_ptr = bc_ptr;
    incoming[i]->username = username;

    outgoing[i] = alloc_worker(sock, outgoing_callback);
    outgoing[i]->state = IDLE;
    outgoing[i]->bc_ptr = NULL;
    outgoing[i]->username = username;
  }

  sleep(1);

  for (i = 0; i < PARALLEL; i++) {
    // this starts them going (INIT state for incoming,
    // IDLE state for outgoing)
    incoming_callback(incoming[i]);
    outgoing_callback(outgoing[i]);
  }

  return sock;
}

void dial_address_server(nng_socket sock, const char *peer_url) {
  int rv;
  if ((rv = (nng_dial(sock, peer_url, NULL, 0)) != 0)) {
    fatal("nng_dial", rv);
  } else {
    printf("Dialed %s\n", peer_url);
  }
}

worker_t *find_idle_outgoing(worker_t *outgoing[]) {
  for (int i = 0; i < PARALLEL; ++i) {
    if (outgoing[i]->state == IDLE) {
      return outgoing[i];
    }
  }
  // shouldn't be reached, theoretically
  return NULL;
}

void mine(blockchain **bc_ptr, const char *username, uint32_t limit,
          worker_t *outgoing[]) {

  // This function will mine `limit` number of blocks.
  // If the limit given is 0, it will mine indefinitely.
  int i = 1;

  while (limit == 0 || i <= limit) {
    block *valid = proof_of_work(*bc_ptr, username, &lock);

    pthread_mutex_lock(&lock);
    if (append_to_blockchain(*bc_ptr, valid)) {
      worker_t *out = find_idle_outgoing(outgoing);
      send_mine_message(*bc_ptr, out);
      char* block_str = to_string_block(((*bc_ptr)->latest_block));
      printf("Mined a block!\n%s\n", block_str);
      free(block_str);
    }
    pthread_mutex_unlock(&lock);
    i++;
  }
  printf("Mining complete.\n");
}

void perform_transaction(worker_t *outgoing[]) {
  char buffer[511];
  uint64_t amount = 0;
  printf("Please enter the amount you wish to transfer: ");
  read_line(buffer, 511);
  amount = (uint64_t) atoi(buffer);
  printf("Please input the username of the person you are transferring to: ");
  read_line(buffer, 511);
  worker_t *out = find_idle_outgoing(outgoing);
  send_transaction_message(buffer, amount, out);
}

void print_state(char *input, blockchain **bc_ptr, const char *username) {
  if (strcmp("bc", input) == 0) {
    pthread_mutex_lock(&lock);
    print_blockchain(*bc_ptr);
    pthread_mutex_unlock(&lock);
  } else if (strcmp("mempool", input) == 0) {
    pthread_mutex_lock(&lock);
    ll_print((*bc_ptr)->mempool, to_string_transaction);
    pthread_mutex_unlock(&lock);
  } else if (strcmp("latest_block", input) == 0) {
    pthread_mutex_lock(&lock);
    print_block((*bc_ptr)->latest_block);
    pthread_mutex_unlock(&lock);
  } else if (strcmp("name", input) == 0) {
    printf("Username: %s\n", username);
  } else if (strcmp("balance", input) == 0){
    uint64_t balance = get_balance(*bc_ptr, username);
    printf("Your balance is: %s%"PRIu64"%s\n",
    (balance == 0) ? BOLDRED : YELLOW, balance, NOCOLOUR);
  } else {
    printf("Invalid command :/\n");
  }
}

void free_and_close_worker(worker_t *w) {
  w->state = IDLE;
  nng_aio_stop(w->aio);
  nng_aio_free(w->aio);
  nng_close(w->sock);
  nng_free(w, sizeof(worker_t));
}

void quit_program(worker_t *incoming[], worker_t *outgoing[]) {
  for (int i = 0; i < PARALLEL; i++) {
    free_and_close_worker(incoming[i]);
    free_and_close_worker(outgoing[i]);
  }
}

int main(int argc, char **argv) {

  worker_t *incoming[PARALLEL];
  worker_t *outgoing[PARALLEL];
  char input[BUFFER_SIZE];

  if (pthread_mutex_init(&lock, NULL) != 0) {
    printf("\n mutex init has failed\n");
    return EXIT_FAILURE;
  }

  blockchain **bc_ptr = malloc(sizeof(blockchain *));
  *bc_ptr = init_blockchain();

  transaction *t1 = init_transaction("wjk", "kgk", 6240, 0);

  ll_append((*bc_ptr)->mempool, t1);


  printf("Please enter a local port to listen on: \n");
  read_line(input, BUFFER_SIZE);

  const char *username = malloc(UID_LENGTH);
  printf("Please enter your username: \n");
  read_line((char *)username, UID_LENGTH);

  nng_socket sock = start_node(input, incoming, outgoing, bc_ptr, username);
  dial_address_server(sock, "tcp://127.0.0.1:8000");
  while (true) {
    fprintf(stdout, "Imp> ");
    read_line(input, BUFFER_SIZE);

    if (is_command('m', input)){
      printf("Proceeding to mine...\n");
      mine(bc_ptr, username, 10, outgoing);

    } else if (is_command('t', input)) {
      printf("Do transaction\n");
      perform_transaction(outgoing);

    } else if (is_command('h', input)){
      printf("Input 'm' to mine, 't' to perform a transaction.\n");

    } else if (is_command('p', input)) {
      print_state(&input[2], bc_ptr, username);
    } else if (is_command('q', input)) {
      quit_program(incoming, outgoing);
      nng_close(sock);
      break;
    }

  }

  free_blockchain(*bc_ptr);
  free(bc_ptr);
  free((char *)username);
  printf("Successfully terminated.\n");
  return EXIT_SUCCESS;
}
