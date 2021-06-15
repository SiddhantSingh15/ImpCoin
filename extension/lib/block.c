#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include <binn.h>

#include "utils.h"
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
  binn_object_set_str(obj, "prev_hash", input->prev_hash);

  return obj;
}

hash *hash_block(block *b);

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

  return new;
}

void serialize_w_hash(binn *b, hash hash) {
  binn_object_set_str(b, "hash", &hash[0]);
}

bool is_valid(block *b);

char *to_string_block(block *b) {

  char buf[511];
  char *out = calloc(10 + b->transactions->size, sizeof(char) * 511);

  if (b->index == 0) {
    sprintf(out, "GENESIS BLOCK - %s\n", b->hash);
  } else {
    printf(out, "Block %03d - %s\n", b->index, b->hash);
  }

  char *fmtedtime = formatted_time(&b->timestamp);
  sprintf(out + strlen(out), " |} mined at: %s\n", fmtedtime);

  to_string_transaction(&b->reward, buf);
  sprintf(out + strlen(out), " |} reward: %s\n", buf);
  sprintf(out + strlen(out), " |} transactions:\n");

  ll_node *curr = b->transactions->head;
  while (curr != NULL) {
    assert(curr->value);
    to_string_transaction(curr->value, buf);
    sprintf(out + strlen(out), " |}  %s\n", buf);
    curr = curr->next;
  }

  sprintf(out + strlen(out), " |} nonce: %"PRIu64"\n", b->nonce);
  sprintf(out + strlen(out), " |} previous hash: %s\n", b->prev_hash);

  out = realloc(out, strlen(out) + 1);
  free(fmtedtime);
  return out;
}

void  print_block(block *b) {
  char *string = to_string_block(b);
  printf("%s", string);
  free(string);
}

void free_block(block *b) {
  ll_free(b->transactions, free_transaction);
  free(b);
}
