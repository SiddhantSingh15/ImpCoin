#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "block.h"
#include "linked_list.h"
#include <stdint.h>
#include <time.h>

typedef struct blockchain {
  block *tail;
  linked_list *mem_pool;
} blockchain;

block_chain *init_blockchain(void);

block *init_block(uint32_t block_index, void *val);

uint32_t append_via_block(block *end_of_chain, void *val);

uint32_t append_to_blockchain(blockchain *chain, void *val);

block *traverse_blockchain(blockchain *chain);

void change_block(blockchain *list, void *val);

void proof_of_work(void);

void free_blockchain(blockchain *list, void (*value_free)(void *));

#endif
