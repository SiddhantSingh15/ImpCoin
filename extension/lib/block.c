#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include <binn.h>
#include <sodium/crypto_generichash.h>
#include <sodium/utils.h>

#include "utils.h"
#include "linked_list.h"
#include "transaction.h"
#include "block.h"

block *init_block(block *prev){
  block *new = calloc(1, sizeof(block));
  new->index = (prev) ? prev->index + 1 : 0;
  new->prev_block = prev;
  new->transactions = ll_init();

  if (prev) {
    memcpy(new->prev_hash, prev->hash, 32);
  }

  return new;
}

binn *serialize_block_no_hash(block *input) {
  binn *obj;
  obj = binn_object();

  binn_object_set_uint32(obj, "index", input->index);
  binn_object_set_uint64(obj, "timestamp", input->timestamp);

  binn *transactions = serialize_transactions(input->transactions);
  binn_object_set_list(obj, "transactions", transactions);
  binn_free(transactions);

  binn *reward = serialize_transaction(&input->reward);
  binn_object_set_object(obj, "reward", reward);
  binn_free(reward);

  binn_object_set_uint64(obj, "nonce", input->nonce);
  binn_object_set_str(obj, "prev_hash", input->prev_hash);

  return obj;
}

binn *serialize_block_w_hash(block *input) {
  // pre: the hash must have been generated already
  assert(input->hash);

  binn *obj;
  obj = serialize_block_no_hash(input);
  binn_object_set_str(obj, "hash", input->hash);

  return obj;
}

hash *hash_block(block *b) {

  binn *serialized = serialize_block_no_hash(b);

  hash *hashed = malloc(sizeof(hash));
  char *out = rand_hash(binn_ptr(serialized), binn_size(serialized));
  memcpy(hashed, out, crypto_generichash_BYTES);
  free(out);

  binn_free(serialized);

  return hashed;
}

block *dup_block(block *b) {
  block *dup = init_block(b->prev_block);
  dup->timestamp = b->timestamp;

  if (b->transactions) {
    dup->transactions = ll_init();
    ll_node *curr = b->transactions->head;
    while (curr != NULL) {
      ll_append(dup->transactions, dup_transaction(curr->value));
    }
  }

  transaction *reward = dup_transaction(&b->reward);
  memcpy(&dup->reward, reward, sizeof(transaction));
  free_transaction(reward);

  dup->nonce = b->nonce;
  memcpy(dup->hash, b->hash, sizeof(hash));

  return dup;
}

block *deserialize_block(binn *b) {
  block *new = calloc(1, sizeof(block));

  new->index = binn_object_uint32(b, "index");
  new->timestamp = binn_object_uint64(b, "timestamp");

  binn *transactions = binn_object_list(b, "transactions");
  new->transactions = deserialize_transactions(transactions);

  transaction *reward = deserialize_transaction(binn_object_object(b, "reward"));
  memcpy(&new->reward, reward, sizeof(transaction));
  free_transaction(reward);

  new->nonce = binn_object_uint64(b, "nonce");

  strncpy(new->prev_hash, binn_object_str(b, "prev_hash"), 32);
  strncpy(new->hash, binn_object_str(b, "hash"), 32);

  return new;
}

bool is_valid(block *b) {
  // pre: the hash has been calculated
  for (int i = 0; i < 3; i++) {
    if (b->hash[i] != '\0') {
      return false;
    }
  }
  return true;
}

char *to_string_block(block *b) {

  char *out = calloc(1, b->transactions->size * sizeof(transaction) + 1000);
  PTR_CHECK(out, "out buffer is null");

  char *hash_string = to_hex_string_hash(&b->hash[0]);
  if (b->index == 0) {
    sprintf(out, "GENESIS BLOCK - 0x%s\n", hash_string);
  } else {
    sprintf(out, "Block %03d - 0x%s\n", b->index, hash_string);
  }
  free(hash_string);

  char *fmtedtime = formatted_time(&b->timestamp);
  sprintf(out + strlen(out), " |} mined at: %s\n", fmtedtime);

  char buf[300];
  to_string_transaction(&b->reward, buf);
  sprintf(out + strlen(out), " |} reward: %s\n", buf);
  sprintf(out + strlen(out), " |} transactions:\n");

  if (b->transactions != NULL) {
    ll_node *curr = b->transactions->head;
    while (curr != NULL) {
      assert(curr->value);
      to_string_transaction(curr->value, buf);
      sprintf(out + strlen(out), " |}  %s\n", buf);
      curr = curr->next;
    }
  }

  sprintf(out + strlen(out), " |} nonce: %"PRIu64"\n", b->nonce);

  char *prev_hash_string = to_hex_string_hash(&b->prev_hash[0]);
  sprintf(out + strlen(out), " |} previous hash: 0x%s\n", prev_hash_string);
  free(prev_hash_string);

  out = realloc(out, strlen(out) + 1);
  free(fmtedtime);
  return out;
}

void print_block(block *b) {
  char *string = to_string_block(b);
  printf("%s", string);
  free(string);
}

void free_block(block *b) {
  if (b->transactions != NULL) {
    ll_free(b->transactions, free_transaction);
  }
  free(b);
}
