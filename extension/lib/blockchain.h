#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <time.h>
#include <pthread.h>

#define GENESIS_BLOCK genesis_block()

typedef struct blockchain {
  block *latest_block;
  linked_list *mempool;
} blockchain;

blockchain *init_blockchain(void);

void free_blockchain(blockchain *chain);

linked_list *init_mempool(void);

void free_mempool(linked_list *mempool);

block *genesis_block(void);

bool append_to_blockchain(blockchain *chain, block *b);

block *traverse_blockchain(blockchain *chain, uint32_t block_num);

binn *serialize_blockchain(blockchain *bc);

blockchain *deserialize_blockchain(binn *input);

block *new_block(blockchain *bc, const char *username);

block *proof_of_work(blockchain *bc, const char *username, pthread_mutex_t *mutex);

bool blockchain_valid (blockchain *curr, blockchain *incoming);

void print_blockchain(blockchain *chain);

char *blockchain_to_string(blockchain *chain);


#endif
