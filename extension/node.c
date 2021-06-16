#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <nng/nng.h>
#include <nng/protocol/bus0/bus.h>
#include <binn.h>
#include <sodium.h>

#include "lib/linked_list.h"
#include "lib/transaction.h"
#include "lib/block.h"
#include "lib/blockchain.h"

#define PARALLEL 32

typedef enum { IDLE, INIT, RECV, WAIT, SEND } state;

struct worker {
  state state;
  nng_aio *aio;
  nng_msg *msg;
  nng_socket sock;
  blockchain *bc;
};

void fatal(const char *func, int rv) {
  fprintf(stderr, "%s: %s\n", func, nng_strerror(rv));
  exit(1);
}

void send_input_message(char *msg, struct worker *w) {
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
  char *buffer;

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
    buffer = (char *) nng_msg_body(w->msg);
    fprintf(stdout, "%s", buffer);
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
    printf("Text message sent");
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

nng_socket start_node(struct worker *incoming[], struct worker *outgoing[]) {
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

  /*
  if ((rv = nng_listen(sock, our_url, NULL, 0)) != 0) {
    fatal("nng_listen", rv);
  } else {
    printf("Listening at %s\n", our_url);
  }
  */

  for (i = 0; i < PARALLEL; i++) {
    incoming[i] = alloc_worker(sock, incoming_callback);
    incoming[i]->state = INIT;
    outgoing[i] = alloc_worker(sock, outgoing_callback);
    outgoing[i]->state = IDLE;
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

int main(int argc, char **argv) {

  struct worker *incoming[PARALLEL];
  struct worker *outgoing[PARALLEL];

  nng_socket sock = start_node(incoming, outgoing);

  char buffer[511];
  while (true) {
    fprintf(stdout, "ASLTY> ");
    for (int i = 0; i < 511; i++) {
      char c = getchar();
      if (c == '\n') {
        buffer[i] = '\0';
        break;
      };
      buffer[i] = c;
    }

    if (buffer[0] == 'd' && buffer[1] == ' ') {
      const char* peer_url = &buffer[2];
      dial_address_server(sock, peer_url);
      continue;
    } else {
      for (int i = 0; i < PARALLEL; ++i) {
        if (outgoing[i]->state == IDLE) {
          send_input_message(buffer, outgoing[i]);
          break;
        }
      }
    }
  }


  // blockchain *bc = init_blockchain();
  // print_block(bc->latest_block);
  printf("Hello, blockchain!\n");
  return EXIT_SUCCESS;
}
