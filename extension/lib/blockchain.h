#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "linked_list.h"
#include <stdint.h>
#include <time.h>

//TODO: define GENESIS_BLOCK. Note : set prev_node to NULL.

typedef struct blockchain {
  block *latest_block;
  linked_list *mem_pool;
} blockchain;

blockchain *init_blockchain(void);

void append_to_blockchain(blockchain *chain, void *val);

block *traverse_blockchain(blockchain *chain, uint32_t block_num);

void proof_of_work(void);

void print_blockchain(blockchain *chain);

void free_blockchain(blockchain *chain);

#endif
