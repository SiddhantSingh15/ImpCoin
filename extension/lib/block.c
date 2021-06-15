#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include <binn.h>

#include "linked_list.h"
#include "transaction.h"
#include "block.h"

block *init_block(block *prev){
  block *new = calloc(1, sizeof(block));
  new->index = (prev) ? prev->index + 1 : 0;
  new->prev_block = prev;

  if (prev) {
    memcpy(new->prev_hash, prev->hash, 32);
  }

  return new;
}

binn *serialize_block(block *input) {
  binn *obj;
  obj = binn_object();

  binn_object_set_uint32(obj, "index", input->index);
  binn_object_set_uint64(obj, "timestamp", input->timestamp);
  binn_object_set_list(obj, "transactions",
    serialize_transactions(input->transactions));
  binn_object_set_object(obj, "reward", serialize_transaction(&input->reward));
  binn_object_set_uint64(obj, "nonce", input->nonce);

  return obj;
}

hash *hash_block(block *b);

block *deserialize_block(binn *b) {
  block *new_block = calloc(1, sizeof(block));
  new_block->index = binn_object_uint32(b, "index");
  new_block->timestamp = binn_object_uint64(b, "timestamp");
  binn *transactions = binn_object_list(b, "transactions");
  memcpy(new_block->transactions, deserialize_transactions(transactions),
    MAX_TRANSACTIONS_PER_BLOCK);
  new_block->reward = *deserialize_transaction(b);
  new_block->nonce = binn_object_uint64(b, "nonce");

  return new_block;
}

void serialize_w_hash(binn *b, hash hash) {
  binn_object_set_str(b, "hash", &hash[0]);
}

bool is_valid(block *b);

void  print_block(block *b) {
  if (b->index == 0) {
    printf("GENESIS BLOCK - %s\n", b->hash);
  } else {
    printf("Block %03d - %s\n", b->index, b->hash);
  }

  char fmttime[100];
  strftime(fmttime, 100, "%m/%d/%y %I:%M%p", localtime(&b->timestamp));
  printf(" |} mined at: %s\n", fmttime);
  printf(" |} nonce: %lu\n", b->nonce);
  printf(" |} previous hash: %s\n", b->prev_hash);
}

void free_block(block *b) {
  free(b);
}
