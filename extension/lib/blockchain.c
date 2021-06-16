#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <binn.h>

#include "linked_list.h"
#include "transaction.h"
#include "block.h"
#include "utils.h"
#include "blockchain.h"

block *genesis_block(void) {
  block *genesis = init_block(NULL);
  genesis->timestamp = -22118400; // 4/20/69, Unix Epoch Time
  genesis->prev_block = NULL;

  // add transactions to give us free money
  transaction *t1 = init_transaction("rick", "ash", 1000, -22118400);
  transaction *t2 = init_transaction("rick", "sid", 1000, -22118400);
  transaction *t3 = init_transaction("rick", "kavya", 1000, -22118400);
  transaction *t4 = init_transaction("rick", "yelun", 1000, -22118400);

  // Create list and add transactions
  genesis->transactions = ll_init();
  ll_append(genesis->transactions, t1);
  ll_append(genesis->transactions, t2);
  ll_append(genesis->transactions, t3);
  ll_append(genesis->transactions, t4);

  hash *genesis_hash = hash_block(genesis);
  memcpy(genesis->hash, genesis_hash, 32);
  free(genesis_hash);
  return genesis;
}

linked_list *init_mempool(void) {
  return ll_init();
}

void free_mempool(linked_list *mem_pool) {
  ll_free(mem_pool, free_transaction);
}

blockchain *init_blockchain(void) {
  blockchain *new = malloc(sizeof(blockchain));
  new->latest_block = GENESIS_BLOCK;
  new->mem_pool = init_mempool();
  return new;
}

void append_to_blockchain(blockchain *chain, block *val){
  assert(chain);
  assert(val);
  assert(is_valid(val));
  chain->latest_block = val;
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

block *new_block(blockchain *bc, char *username) {
  block *new = init_block(bc->latest_block);

  int i = 0;
  ll_node *curr = bc->mem_pool->head;
  while (curr != NULL && i < MAX_TRANSACTIONS_PER_BLOCK) {
    assert(curr->value);
    ll_append(new->transactions, dup_transaction((transaction *)curr->value));
  }

  new->timestamp = time(NULL);

  transaction *reward = init_transaction("rick", username, 69, time(NULL));
  memcpy(&new->reward, reward, sizeof(transaction));
  free_transaction(reward);

  new->nonce = random_long();
  return new;
}

block *proof_of_work(blockchain *bc, char *username) {
  block *new = new_block(bc, username);

  hash *new_hash = hash_block(new);
  strncpy(new->hash, *new_hash, 32);
  free(new_hash);

  while (!is_valid(new)) {
    free_block(new);
    new = new_block(bc, username);

    hash *new_hash = hash_block(new);
    memcpy(new->hash, *new_hash, 32);
    free(new_hash);
  }

  return new;
}

void free_blockchain(blockchain *chain){
  assert(chain);

  // Free the mempool
  free_mempool(chain->mem_pool);

  // Free the blocks one-by-one
  block *curr = chain->latest_block;
  while(curr != NULL){
    block *temp = curr;
    curr = curr->prev_block;
    free_block(temp);
  }
  free(chain);
}

char *blockchain_to_string(blockchain *chain) {
  assert(chain);
  assert(chain->latest_block);
  char buf[600];
  block *curr = chain->latest_block;
  char *out = calloc(curr->index + 1, sizeof(buf));
  while (curr != NULL) {
    assert(curr);
    strcpy(buf, to_string_block(curr));
    sprintf(out + strlen(out), "%s", buf);
    if (curr->prev_block != NULL) {
      sprintf(out + strlen(out), "%s\n", "----- NEXT BLOCK -----");
    }
    curr = curr->prev_block;
  }
  out = realloc(out, strlen(out) + 1);
  return out;
}

void print_blockchain(blockchain *chain) {
  char *string = blockchain_to_string(chain);
  printf("%s", string);
  free(string);
}