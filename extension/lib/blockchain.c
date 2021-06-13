#include "linked_list.h"
#include "block.h"
#include "blockchain.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

blockchain *init_block_chain(void) {
  blockchain *new_chain = malloc(sizeof(blockchain));
  new_chain->tail = GENESIS_BLOCK;
  new_chain->tail->index = 0;
  new_chain->mem_pool = NULL;
  return new_chain;
}

void append_to_blockchain(blockchain *chain, void *val){
  assert(chain);
  assert(val);

  block *to_append = init_block(chain->tail, chain->tail->index + 1, val);
  to_append->prev_hash = chain->tail->this_hash;
  //TODO: set this_hash and other missing attributes
  chain->tail = to_append; 
 }

block *traverse_blockchain(blockchain *chain, uint32_t block_num){
  assert(chain);
  assert(0 <= block_num <= chain->tail->index);

  block *curr = chain->tail;
  
  while (curr->index != block_num){
    curr = curr->prev_block;
  }
  return curr;
}

void proof_of_work(void);

void free_blockchain(blockchain *chain, void (*value_free)(void *)){
  assert(chain);
  assert(value_free);

  block *curr = chain->tail;
  while(curr->prev_block != NULL){
    block *temp = curr;
    curr = curr->prev_block;
    //TODO: free all attributes of temp.
    value_free(temp->value);
    free(temp);
  }
  free(chain);
}
