#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>
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
  hash *this_hash;
  hash *prev_hash;
} block;

#endif
