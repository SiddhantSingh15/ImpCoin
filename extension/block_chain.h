#ifndef BLOCK_CHAIN_H
#define BLOCK_CHAIN_H

#include "block.h"
#include "linked_list.h"
#include <stdint.h>
#include <time.h>

typedef struct block_chain {
  block *tail;
  linked_list *mem_pool;
} block_chain;

block_chain *init_block_chain(void);

block *init_block(uint32_t block_index, void *val);

uint32_t append_via_block(block *end_of_chain, void *val);

uint32_t append_to_block_chain(block_chain *chain, void *val);

block *traverse_block_chain(block_chain *chain);

void change_block(block_chain *list, void *val);

void free_block_chain(block_chain *list, void (*value_free)(void *));

#endif
