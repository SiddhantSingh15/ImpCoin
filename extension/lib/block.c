#include "transaction.h"
#include "block.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

block *init_block(block *prev){
  block *new = calloc(1, sizeof(block));
  new->index = (prev) ? prev->index + 1 : 0;
  new->prev_block = prev;

  if (prev) {
    memcpy(new->prev_hash, prev->hash, 32);
  }

  return new;
}

hash *hash_block(block *b);

void deserialize_block(block *b);

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
