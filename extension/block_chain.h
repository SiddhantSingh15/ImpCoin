#ifndef BLOCK_CHAIN_H
#define BLOCK_CHAIN_H

#include <stdint.h>

typedef char *hash;

/**
 * @brief The design of a block that can be stored in the block-chain.
 *        It stores the value of the block, the block number as it's index, 
 *        the hash of the block and the hash of the pevious block.
 */
typedef struct block {
  void *value;
  uint32_t index;
  hash *this_hash;
  hash *prev_hash;
} block;

typedef struct block_chain {
  hash *tail;
} block_chain;

block_chain *init_block_chain(void);

block *init_block(uint32_t address, void *val);

uint32_t append_via_block(block *entry, void *val);

uint32_t append_to_block_chain(block_chain *chain, void *val);

block *traverse_block_chain(block_chain *chain);

void change_block(block_chain *list, uint32_t address, void *val);

void free_block_chain(block_chain *list, void (*value_free)(void *));

#endif
