#include "blockchain.h"
#include "block.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

//TODO: define genesis block

blockchain *init_block_chain(void) {
  blockchain *new_chain = malloc(sizeof(blockchain));
  new_chain->tail = GENESIS_BLOCK;
  new_chain->mem_pool = NULL;
  return new_chain;
}

uint32_t append_to_blockchain(blockchain *chain, void *val);

