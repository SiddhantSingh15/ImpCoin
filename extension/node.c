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

#define PARALLEL 32

// We only dereference a bc_ptr when the lock is acquired
pthread_mutex_t lock;

typedef enum { IDLE, INIT, RECV, WAIT, SEND } state;

struct worker {
  state state;
  nng_aio *aio;
  nng_msg *msg;
  nng_socket sock;
  blockchain **bc_ptr;
  const char *username;
};

void fatal(const char *func, int rv) {
  fprintf(stderr, "%s: %s\n", func, nng_strerror(rv));
  exit(1);
}

void send_mine_message(blockchain *bc, struct worker *w) {
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
}

void send_transaction_message(char *msg, struct worker *w) {
  int rv;
  int msg_size;
  nng_msg *nng_msg;

  w->state = SEND;
  if ((rv = nng_msg_alloc(&nng_msg, 0)) != 0) {
    fatal("nng_msg_alloc", rv);
  }

  msg_size = strlen(msg) + 1;
  nng_msg_insert(nng_msg, msg, msg_size);
  nng_aio_set_msg(w->aio, nng_msg);
  nng_send_aio(w->sock, w->aio);
}

void incoming_callback(void *arg) {
  struct worker *w = arg;
  int rv;
  binn *buffer;
  char type[10];

  switch (w->state) {
  case INIT:
    w->state = RECV;
    nng_recv_aio(w->sock, w->aio);
    break;
  case RECV:
    if ((rv = nng_aio_result(w->aio)) != 0) {
      fatal("nng_recv_aio", rv);
    }

    w->msg = nng_aio_get_msg(w->aio);
    buffer = (binn *)nng_msg_body(w->msg);
    strcpy(type, binn_object_str(buffer, "type"));

    if (strcmp(w->username, binn_object_str(buffer, "username")) == 0) {
      // This was sent by us, ignore
      break;
    }

    if (strcmp(type, "mine") == 0) {
      printf("I received a block from %s\n", binn_object_str(buffer, "username"));
      blockchain_msg *bc_msg = deserialize_bc_msg(buffer);

      // lock
      pthread_mutex_lock(&lock);

      if (blockchain_valid(*w->bc_ptr, bc_msg->bc)) {
        // blockchain *to_free = *w->bc_ptr;
        *w->bc_ptr = bc_msg->bc;
        // free_blockchain(to_free);
        // printf("%s\n", blockchain_to_string(*w->bc));
        print_block((*(w->bc_ptr))->latest_block);
      } else {
        free_blockchain(bc_msg->bc);
      }

      // unlock
      pthread_mutex_unlock(&lock);

    } else {
      printf("This is supposed to be for transactions\n");
    }

    nng_recv_aio(w->sock, w->aio);
    w->state = RECV;
    break;
  default:
    break;
  }
}

void outgoing_callback(void *arg) {
  struct worker *w = arg;

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

struct worker *alloc_worker(nng_socket sock, void (* callback)(void *)) {
  struct worker *w;
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

nng_socket start_node(const char *our_url, struct worker *incoming[],
                      struct worker *outgoing[], blockchain **bc_ptr,
                      const char *username) {
  nng_socket sock;
  int rv;
  int i;

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

struct worker *find_idle_outgoing(struct worker *outgoing[]) {
  for (int i = 0; i < PARALLEL; ++i) {
    if (outgoing[i]->state == IDLE) {
      return outgoing[i];
    }
  }
  // shouldn't be reached, theoretically
  return NULL;
}

void mine(blockchain **bc_ptr, const char *username, uint32_t limit,
          struct worker *outgoing[]) {

  // This function will mine `limit` number of blocks.
  // If the limit given is 0, it will mine indefinitely.
  int i = 1;

  while (limit == 0 || i <= limit) {
    // lock
    // pthread_mutex_lock(&lock);

    block *valid = proof_of_work(*bc_ptr, username, &lock);

    pthread_mutex_lock(&lock);
    if (append_to_blockchain(*bc_ptr, valid)) {
      struct worker *out = find_idle_outgoing(outgoing);
      send_mine_message(*bc_ptr, out);
    }
    pthread_mutex_unlock(&lock);

    // unlock
    // pthread_mutex_unlock(&lock);
    i++;
  }
}

int main(int argc, char **argv) {

  struct worker *incoming[PARALLEL];
  struct worker *outgoing[PARALLEL];
  char input_buf[511];

  if (pthread_mutex_init(&lock, NULL) != 0) {
    printf("\n mutex init has failed\n");
    return EXIT_FAILURE;
  }

  blockchain **bc_ptr = malloc(sizeof(blockchain *));
  *bc_ptr = init_blockchain();

  transaction *t1 = init_transaction("rick", "george", 420, 0);
  transaction *t2 = init_transaction("rick", "george", 4200, 0);
  transaction *t3 = init_transaction("rick", "george", 4220, 0);
  transaction *t4 = init_transaction("rick", "george", 4240, 0);

  transaction *t5 = init_transaction("rick", "george", 5220, 0);
  transaction *t6 = init_transaction("rick", "george", 6240, 0);

  ll_append((*bc_ptr)->mempool, t1);
  ll_append((*bc_ptr)->mempool, t2);
  ll_append((*bc_ptr)->mempool, t3);
  ll_append((*bc_ptr)->mempool, t4);

  ll_append((*bc_ptr)->mempool, t5);
  ll_append((*bc_ptr)->mempool, t6);


  printf("Please enter your local ip port thing: \n");
  read_line(input_buf, 511);

  const char *username = malloc(511);
  printf("Please enter your username: \n");
  read_line((char *)username, 511);

  nng_socket sock = start_node(input_buf, incoming, outgoing, bc_ptr, username);
  dial_address_server(sock, "tcp://127.0.0.1:8000");
  while (true) {
    fprintf(stdout, "ASLTY> ");
    read_line(input_buf, 511);

    mine(bc_ptr, username, 10, outgoing);
    /*
    struct worker *out = find_idle_outgoing(outgoing);
    send_input_message(input_buf, out);
    */
  }

  free_blockchain(*bc_ptr);
  free(bc_ptr);
  free((char *)username);

  // blockchain *bc = init_blockchain();
  // print_block(bc->latest_block);
  printf("Hello, blockchain!\n");
  return EXIT_SUCCESS;
}
