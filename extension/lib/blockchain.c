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

void free_mempool(linked_list *mempool) {
  ll_free(mempool, free_transaction);
}

blockchain *init_blockchain(void) {
  blockchain *new = malloc(sizeof(blockchain));
  new->latest_block = GENESIS_BLOCK;
  new->mempool = init_mempool();
  return new;
}

bool append_to_blockchain(blockchain *chain, block *b){
  assert(chain && b);

  // Append the new block to the blockchain
  if (is_valid(b) && chain->latest_block == b->prev_block) {
    chain->latest_block = b;

    // Remove the transactions in the block from the mempool.
    // The transactions in the block are always taken from the head of the
    // mempool, so we can just drop that number of nodes from it.
    ll_drop(chain->mempool, (b->transactions) ? b->transactions->size : 0,
            free_transaction);
    return true;
  }
  return false;
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

// ---------------------------------------------------------------------------
// Serialize and deserialize functions
//

binn *serialize_blockchain(blockchain *bc) {
  binn *obj;
  obj = binn_object();

  binn *mempool = serialize_transactions(bc->mempool);
  binn_object_set_list(obj, "mempool", mempool);
  binn_free(mempool);

  binn *blockchain = binn_list();
  block *curr = bc->latest_block;
  while (curr != NULL) {
    binn *block = serialize_block_w_hash(curr);
    binn_list_add_object(blockchain, block);
    binn_free(block);
    curr = curr->prev_block;
  }
  binn_object_set_list(obj, "blockchain", blockchain);
  binn_free(blockchain);

  return obj;
}

blockchain *deserialize_blockchain(binn *input) {
  blockchain *bc = malloc(sizeof(blockchain));

  binn *mempool = binn_object_list(input, "mempool");
  bc->mempool = deserialize_transactions(mempool);

  binn *blockchain = binn_object_list(input, "blockchain");
  binn_iter iter;
  binn value;
  binn_list_foreach(blockchain, value) {
    block *b = deserialize_block(&value);
    b->prev_block = bc->latest_block;
    bc->latest_block = b;
  }

  return bc;
}

// ---------------------------------------------------------------------------
// Proof of work and mining
//
block *new_block(blockchain *bc, char *username) {
  block *new = init_block(bc->latest_block);

  int i = 0;
  ll_node *curr = bc->mempool->head;
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
  free_mempool(chain->mempool);

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
  assert(chain && chain->latest_block);

  block *curr = chain->latest_block;
  char *out = calloc(curr->index + 1, 800);

  while (curr != NULL) {
    char *block_string = to_string_block(curr);
    sprintf(out + strlen(out), "%s", block_string);

    if (curr->prev_block != NULL) {
      sprintf(out + strlen(out), "%s\n", " |}");
    }

    free(block_string);
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
