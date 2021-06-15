#include <stdio.h>
#include <stdlib.h>

#include <nng/nng.h>
#include <binn.h>
#include <sodium.h>

#include "lib/linked_list.h"
#include "lib/transaction.h"
#include "lib/block.h"
#include "lib/blockchain.h"

int main(int argc, char **argv) {
  blockchain *bc = init_blockchain();
  print_block(bc->latest_block);
  printf("Hello, blockchain!\n");
  return EXIT_SUCCESS;
}
