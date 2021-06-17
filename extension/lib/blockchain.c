#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

#include <binn.h>

#include "../definitions.h"
#include "linked_list.h"
#include "transaction.h"
#include "block.h"
#include "blockchain.h"
#include "utils.h"

/**---------------------------------------------------------------------------
 * Init and Free
 *--------------------------------------------------------------------------*/

blockchain *init_blockchain(void) {
  blockchain *new = malloc(sizeof(blockchain));
  new->latest_block = GENESIS_BLOCK;
  new->mempool = init_mempool();
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

linked_list *init_mempool(void) {
  return ll_init();
}

void free_mempool(linked_list *mempool) {
  ll_free(mempool, free_transaction);
}

/**---------------------------------------------------------------------------
 * Genesis Block
 *--------------------------------------------------------------------------*/


block *genesis_block(void) {
  block *genesis = init_block(NULL);
  genesis->timestamp = GENESIS_TIME; // 4/20/69, Unix Epoch Time
  genesis->prev_block = NULL;

  // add transactions to give us free money
  transaction *t1 = init_transaction("wjk", "ash", 1000, GENESIS_TIME);
  transaction *t2 = init_transaction("wjk", "sid", 1000, GENESIS_TIME);
  transaction *t3 = init_transaction("wjk", "kavya", 1000, GENESIS_TIME);
  transaction *t4 = init_transaction("wjk", "yelun", 1000, GENESIS_TIME);

  transaction *reward =
      init_transaction("docsoc", (char *)"wjk", 21000000, GENESIS_TIME);
  memcpy(&genesis->reward, reward, sizeof(transaction));
  free_transaction(reward);

  // Create list and add transactions
  ll_append(genesis->transactions, t1);
  ll_append(genesis->transactions, t2);
  ll_append(genesis->transactions, t3);
  ll_append(genesis->transactions, t4);

  hash *genesis_hash = hash_block(genesis);
  memcpy(genesis->hash, genesis_hash, HASH_SIZE);
  free(genesis_hash);
  return genesis;
}

/**---------------------------------------------------------------------------
 * Adding and Traversing the Blockchain
 *--------------------------------------------------------------------------*/

bool append_to_blockchain(blockchain *chain, block *b){
  assert(chain && b);

  // Append the new block to the blockchain
  if (is_valid_block(b) && chain->latest_block == b->prev_block) {
    chain->latest_block = b;

    // Remove the transactions in the block from the mempool.
    // The transactions in the block are always taken from the head of the
    // mempool, so we can just drop that number of nodes from it.
    ll_drop(chain->mempool, b->transactions->size, free_transaction);
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

/**---------------------------------------------------------------------------
 * Serialize / Deserialize Functions
 *--------------------------------------------------------------------------*/

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
  int count = binn_count(blockchain);
  bc->latest_block = NULL;
  for(int i = count; i > 0; i--) {
    block *b = deserialize_block(binn_list_object(blockchain, i));
    b->prev_block = bc->latest_block;
    bc->latest_block = b;
  }

  return bc;
}

/**---------------------------------------------------------------------------
 * Proof of work & Consensus
 *--------------------------------------------------------------------------*/

block *new_block(blockchain *bc, const char *username) {
  block *new = init_block(bc->latest_block);

  int i = 0;
  ll_node *curr = bc->mempool->head;
  while (i < MAX_TRANSACTIONS_PER_BLOCK && curr != NULL) {
    assert(curr->value);
    ll_append(new->transactions, dup_transaction((transaction *)curr->value));
    curr = curr->next;
    i++;
  }

  new->timestamp = time(NULL);

  transaction *reward =
      init_transaction("wjk", (char *)username, 69, time(NULL));
  memcpy(&new->reward, reward, sizeof(transaction));
  free_transaction(reward);

  new->nonce = random_long();
  return new;
}

block *proof_of_work(blockchain *bc, const char *username,
                     pthread_mutex_t *mutex) {
  block *new = NULL;
  int count = 0;

  do {
    if (new != NULL){
      free_block(new);
    }
    pthread_mutex_lock(mutex);
    new = new_block(bc, username);
    pthread_mutex_unlock(mutex);

    hash *new_hash = hash_block(new);
    memcpy(new->hash, *new_hash, HASH_SIZE);
    free(new_hash);
    count++;
  } while (!is_valid_block(new));
  return new;
}

bool blockchain_valid(blockchain *curr, blockchain *incoming) {
  block *latest_curr = curr->latest_block;

  if (curr->latest_block->index > incoming->latest_block->index) {
    return false;
  }

  block *new_b = incoming->latest_block;
  while (new_b->index > 0) {

    // If previous block's hash and prev hash don't match
    if (memcmp(new_b->prev_hash, new_b->prev_block->hash, HASH_SIZE) != 0) {
      return false;
    }

    // If curr exists within new
    if (new_b->index == latest_curr->index &&
        memcmp(new_b->hash, latest_curr->hash, HASH_SIZE) == 0) {
      return true;
    }

    new_b = new_b->prev_block;
  }
  return true;
}

/**---------------------------------------------------------------------------
 * Balance
 *--------------------------------------------------------------------------*/
uint64_t get_balance(blockchain *bc, const char *username) {
  uint64_t amount = 0;
  block *curr_b = bc->latest_block;
  ll_node *curr_lln;
  while (curr_b != NULL) {
    curr_lln = (curr_b->transactions) ? curr_b->transactions->head : NULL;
    while (curr_lln != NULL) {
      transaction *trans = (transaction *) curr_lln->value;
      if (strcmp(trans->to, username) == 0) {
        amount += trans->amount;
      } else if (strcmp(trans->from, username) == 0) {
        amount -= trans->amount;
      }
      curr_lln = curr_lln->next;
    }
    amount += strcmp(curr_b->reward.to, username) == 0 ? curr_b->reward.amount : 0;
    curr_b = curr_b->prev_block;
  }
  return amount;
}
/**---------------------------------------------------------------------------
 * To String and Print
 *--------------------------------------------------------------------------*/

char *blockchain_to_string(blockchain *chain) {
  assert(chain && chain->latest_block);

  block *curr = chain->latest_block;
  char *out = calloc(curr->index + 1, LARGE_BUFFER_SIZE);

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
