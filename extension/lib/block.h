#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef char *hash;

/**
 * @brief The design of a block that can be stored in the block-chain.
 *        It stores the value of the block, the block number as it's index, 
 *        the hash of the block and the hash of the pevious block.
 */
typedef struct block {
  void *value;
  uint32_t index;
  time_t timestamp;
  long *nons;
  block *prev_block;
  hash *this_hash;
  hash *prev_hash;
} block;

block *init_block(block *prev, uint32_t block_num, void *val);

void hash_block(block *b);

void deserialize_block(block *b);

bool is_valid(block *b);

#endif
