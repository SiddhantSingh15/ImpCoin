#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "linked_list.h"
#include <stdint.h>
#include <time.h>

#define GENESIS_BLOCK genesis_block()

typedef struct blockchain {
  block *latest_block;
  linked_list *mem_pool;
} blockchain;

block *genesis_block(void);

blockchain *init_blockchain(void);

void append_to_blockchain(blockchain *chain, block *val);

block *traverse_blockchain(blockchain *chain, uint32_t block_num);

block *proof_of_work(blockchain *bc, char *username);

void print_blockchain(blockchain *chain);

void free_blockchain(blockchain *chain);

char *blockchain_to_string(blockchain *chain);

#endif
