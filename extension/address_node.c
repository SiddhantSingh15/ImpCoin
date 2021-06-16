#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <nng/nng.h>
#include <nng/protocol/bus0/bus.h>
#include <nng/supplemental/util/platform.h>
#include <binn.h>
#include <sodium.h>

#include "lib/linked_list.h"
#include "lib/transaction.h"
#include "lib/block.h"
#include "lib/blockchain.h"

#define PARALLEL 32

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
  char *buffer;

	switch (w->state) {
	case INIT:
    fprintf(stdout,"%s","INIT");
		w->state = RECV;
		nng_recv_aio(w->sock, w->aio);
		break;
	case RECV:
    printf("???");
		if ((rv = nng_aio_result(w->aio)) != 0) {
			fatal("nng_recv_aio", rv);
		}
		w->msg = nng_aio_get_msg(w->aio);
    buffer = (char *) nng_msg_body(w->msg);
    printf("%s", buffer);
		w->state = WAIT;
		break;
	case WAIT:
		nng_aio_set_msg(w->aio, w->msg);
		w->msg   = NULL;
		w->state = SEND;
		nng_send_aio(w->sock, w->aio);
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
  fprintf(stdout,"?");
  if ((w = nng_alloc(sizeof(*w))) == NULL) {
    fatal("nng_alloc", NNG_ENOMEM);
  }

  if ((rv = nng_aio_alloc(&w->aio, address_callback, w)) != 0) {
    fatal("nng_aio_alloc", rv);
  }
  w->sock = sock;
  w->state = INIT;
  return w;
}

void start_address_node(const char *our_url, struct addr_worker *workers[]) {
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

  printf("1");
  if ((rv = nng_listen(sock, our_url, NULL, 0)) != 0) {
    fatal("nng_listen", rv);
  } else {
    printf("Listening at %s\n", our_url);
    printf("2");
  }
  for (i = 0; i < PARALLEL; i++) {
    printf("%d", i);
    workers[i] = alloc_worker(sock);
  }
  sleep(1);

  for (i = 0; i < PARALLEL; i++) {
    address_callback(workers[i]);
  }
  for (;;) {
    nng_msleep(3600000);
  }
}

int main(int argc, char **argv) {

  struct addr_worker *workers[PARALLEL];
  const char *host_url = "tcp://127.0.0.1:8000\0";

  // blockchain *bc = init_blockchain();
  // print_block(bc->latest_block);
  printf("Hello, blockchain!\n");

  start_address_node(host_url, workers);
  return EXIT_SUCCESS;
}
