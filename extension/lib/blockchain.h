#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "linked_list.h"
#include <stdint.h>
#include <time.h>

#define GENESIS_BLOCK genesis_block()

typedef struct blockchain {
  block *latest_block;
  linked_list *mempool;
} blockchain;

block *genesis_block(void);

blockchain *init_blockchain(void);

bool append_to_blockchain(blockchain *chain, block *b);

block *traverse_blockchain(blockchain *chain, uint32_t block_num);

binn *serialize_blockchain(blockchain *bc);

blockchain *deserialize_blockchain(binn *input);

block *new_block(blockchain *bc, const char *username);

block *proof_of_work(blockchain *bc, const char *username, pthread_mutex_t *mutex);

void print_blockchain(blockchain *chain);

void free_blockchain(blockchain *chain);

char *blockchain_to_string(blockchain *chain);

bool blockchain_valid (blockchain *curr, blockchain *incoming);

#endif
