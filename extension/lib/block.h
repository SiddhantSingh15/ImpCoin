#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define MAX_TRANSACTIONS_PER_BLOCK 3

typedef char hash[32];

/**
 * @brief The design of a block that can be stored in the block-chain.
 *        It stores the value of the block, the block number as it's index,
 *        the hash of the block and the hash of the pevious block.
 */
typedef struct block {
  uint32_t index;
  time_t timestamp;
  transaction transactions[MAX_TRANSACTIONS_PER_BLOCK];
  transaction reward;
  uint64_t nonce;
  hash prev_hash;
  hash hash; // Hash of all the fields above this
  struct block *prev_block; // Ignore for serialise and hash
} block;

block *init_block(block *prev);

hash *hash_block(block *b);

void deserialize_block(block *b);

bool is_valid(block *b);

void print_block(block *b);

void free_block(block *b);

#endif
