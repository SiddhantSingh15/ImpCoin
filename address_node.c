#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <nng/nng.h>
#include <nng/protocol/bus0/bus.h>
#include <nng/supplemental/util/platform.h>
#include <binn.h>
#include <sodium.h>

#include "lib/linked_list.h"
#include "lib/transaction.h"
#include "lib/block.h"
#include "lib/blockchain.h"
#include "lib/messages.h"

typedef enum { IDLE, INIT, RECV, WAIT, SEND } state;

struct addr_worker {
  state state;
  nng_aio *aio;
  nng_msg *msg;
  nng_socket sock;
};

void fatal(const char *func, int rv) {
  fprintf(stderr, "%s: %s\n", func, nng_strerror(rv));
  exit(1);
}

void address_callback(void *arg) {

  struct addr_worker *w = arg;
  int rv;
  nng_msg *msg;
  binn *buffer;
  char type[MESSAGE_TYPE_SIZE];

  switch (w->state) {
  case INIT:
    w->state = RECV;
    nng_recv_aio(w->sock, w->aio);
    break;
  case RECV:
    if ((rv = nng_aio_result(w->aio)) != 0) {
			fatal("nng_recv_aio", rv);
		}
    msg = nng_aio_get_msg(w->aio);

    if ((rv = nng_aio_result(w->aio)) != 0) {
      fatal("nng_recv_aio", rv);
    } else {
      printf("Result received\n");
    }

    w->msg = msg;
    buffer = (binn *)nng_msg_body(w->msg);
    strcpy(type, binn_object_str(buffer, "type"));
    if (strcmp(type, "mine") == 0) {
      blockchain_msg *bc_msg = deserialize_bc_msg(buffer);
      print_blockchain(bc_msg->bc);
      free_blockchain(bc_msg->bc);
      free(bc_msg);
    } else if (strcmp(type, "trans") == 0) {
      printf("I've received a transaction\n");
      transaction_msg *t_msg = deserialize_t_msg(buffer);
      printf(
        "It is from %s, attempting to transfer %"PRIu64" ImpCoin to %s.\n",
        t_msg->username,t_msg->amount, t_msg->to
      );
      free(t_msg);
    }
    nng_aio_set_msg(w->aio, w->msg);
    w->msg = NULL;
    w->state = SEND;
    nng_send_aio(w->sock, w->aio);
    nng_msg_free(w->msg);
    break;
  case SEND:
    // If message is sent successfuly, nng_aio_result returns 0
    if ((rv = nng_aio_result(w->aio)) != 0) {
      nng_msg_free(w->msg);
      fatal("nng_send_aio", rv);
    }
    w->state = RECV;
    nng_recv_aio(w->sock, w->aio);
    break;
  default:
    fatal("bad state!", NNG_ESTATE);
    break;
  }
}

struct addr_worker *alloc_worker(nng_socket sock) {

  struct addr_worker *w;
  int rv;

  if ((w = nng_alloc(sizeof(*w))) == NULL) {
    fatal("nng_alloc", NNG_ENOMEM);
  }
  if ((rv = nng_aio_alloc(&w->aio, address_callback, w)) != 0) {
    fatal("nng_aio_alloc", rv);
  }
  w->state = INIT;
  w->sock = sock;
  return (w);
}

int run_address_node(const char *our_url, struct addr_worker *workers[]) {

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

  for (i = 0; i < PARALLEL; i++) {
    workers[i] = alloc_worker(sock);
  }

  if ((rv = nng_listen(sock, our_url, NULL, 0)) != 0) {
    fatal("nng_listen", rv);
  }

  for (i = 0; i < PARALLEL; i++) {
    address_callback(workers[i]);
  }

  sleep(1);

  return 0;
}

int main(int argc, char **argv) {

  printf("ImpAddressServer, Version 0.1\n");
  struct addr_worker *workers[PARALLEL];
  const char *host_url = "tcp://127.0.0.1:8000";
  int rc = run_address_node(host_url, workers);

  while (true);
  exit(rc == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
