#include "linked_list.h"
#include "transaction.h"
#include "block.h"
#include "blockchain.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define GENESIS_BLOCK genesis_block()

block *genesis_block(void) {
  // TODO: Not implemented
  return NULL;
}

blockchain *init_block_chain(void) {
  blockchain *new_chain = malloc(sizeof(blockchain));
  new_chain->latest_block = GENESIS_BLOCK;
  new_chain->latest_block->index = 0;
  new_chain->mem_pool = NULL;
  return new_chain;
}

void append_to_blockchain(blockchain *chain, void *val){
  assert(chain);
  assert(val);

  block *to_append = init_block(chain->latest_block);
  memcpy(to_append->prev_hash, chain->latest_block->hash, 32);
  //TODO: set this_hash and other missing attributes
  chain->latest_block = to_append;
 }

block *traverse_blockchain(blockchain *chain, uint32_t block_num){
  assert(chain);
  assert(0 <= block_num && block_num <= chain->latest_block->index);

  block *curr = chain->latest_block;

  while (curr->index != block_num){
    curr = curr->prev_block;
  }
  return curr;
}

void proof_of_work(void);

void free_blockchain(blockchain *chain){
  assert(chain);

  block *curr = chain->latest_block;
  while(curr->prev_block != NULL){
    block *temp = curr;
    curr = curr->prev_block;
    //TODO: free all attributes of temp.
    free(temp);
  }
  free(chain);
}
