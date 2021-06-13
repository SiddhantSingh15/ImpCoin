#include "block.h"
#include <stdio.h>
#include <stdlib.h>

block *init_block(void){
  block *new_block = calloc(1, sizeof(block));
  new_block->value = NULL;
  new_block->this_hash = NULL;
  new_block->prev_hash = NULL;
  return new_block;
}

void hash_block(block *b);

void deserialize_block(block *b);

bool is_valid(block *b);


