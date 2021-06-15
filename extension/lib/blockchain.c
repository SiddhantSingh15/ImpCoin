#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <binn.h>

#include "linked_list.h"
#include "transaction.h"
#include "block.h"
#include "blockchain.h"

#define GENESIS_BLOCK genesis_block()

block *genesis_block(void) {
  block *genesis = init_block(NULL);
  genesis->timestamp = -22118400; // 4/20/69, Unix Epoch Time
  // add transactions to give us free money
  // hash *genesis_hash = hash_block(genesis);
  memcpy(genesis->hash, "its a hash hahaha lmao", 32);
  // free(genesis_hash);
  return genesis;
}

linked_list *init_mempool(void) {
  return init_linked_list();
}

void free_mempool(linked_list *mem_pool) {
  // TODO: do we need a specialised `free_transaction` ?
  free_linked_list(mem_pool, free);
}

blockchain *init_blockchain(void) {
  blockchain *new = malloc(sizeof(blockchain));
  new->latest_block = GENESIS_BLOCK;
  new->mem_pool = init_mempool();
  return new;
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

  // Free the mempool
  free_mempool(chain->mem_pool);

  // Free the blocks one-by-one
  block *curr = chain->latest_block;
  while(curr->prev_block != NULL){
    block *temp = curr;
    curr = curr->prev_block;
    free_block(temp);
  }
  free(chain);
}
