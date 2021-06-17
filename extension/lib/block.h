#ifndef BLOCK_H
#define BLOCK_H

#include <binn.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef char hash[32];

/**
 * @brief The design of a block that can be stored in the block-chain.
 *        It stores the value of the block, the block number as it's index,
 *        the hash of the block and the hash of the pevious block.
 */
typedef struct block {
  uint32_t index;
  time_t timestamp;
  linked_list *transactions;
  transaction reward;
  uint64_t nonce;
  hash prev_hash;
  hash hash; // Hash of all the fields above this
  struct block *prev_block; // Ignore for serialise and hash
} block;


block *init_block(block *prev);

block *dup_block(block *b);

void free_block(block *b);

binn *serialize_block_no_hash(block *input);

binn *serialize_block_w_hash(block *input);

block *deserialize_block(binn *b);

hash *hash_block(block *b);

bool is_valid_block(block *b);

char *to_string_block(block *b);

void print_block(block *b);

#endif
