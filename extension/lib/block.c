#include "transaction.h"
#include "block.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void hash_block(block *b);

void deserialize_block(block *b);

bool is_valid(block *b);
