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
 *        the hash of the block and the hash of the pevious block. It also 
 *        stores the timestamp of it's creation, a linked list of pending 
 *        transactions, a reward transaction, a nonce value (nonsense value), 
 *        and a pointer to the previous block in the blockchain. 
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

/**
 * @brief Creates (using calloc) and initializes a block, given the block
 *  previous to it in the blockchain.
 *
 * @param prev The block immediately before the block that is being
 *  initialized in the blockchain. The prev_block attribute of the new 
 *  block is initialized with prev.
 *
 * @return The new block after initializing its relevant attributes.
 */
block *init_block(block *prev);

/**
 * @brief Duplicates a given block using init_block and returns the duplicated 
 *  block.
 *
 * @param b The block to be duplicated.
 *
 * @return The duplicated block.
 */
block *dup_block(block *b);

/**
 * @brief Frees the memory space allocated to a given block.
 *
 * @param b The block whose allocated memory is to be freed.
 */
void free_block(block *b);

/**
 * @brief Creates a new binn object and serializes the block by initializing 
 *  the binn object parameters using the block attributes, ewithout the hash 
 *  of the block.
 *
 * @param input The block to be serialized.
 *
 * @return The new binn object (serialized block).
 */
binn *serialize_block_no_hash(block *input);

/**
 * @brief Serializes a block using serialize_block_no_hash but also allocates 
 *  the hash of the block in the binn object. We assume the block already 
 *  contains a hash.
 *
 * @param input The block to be serialized, including its hash.
 *
 * @return The serialized block, including the hash.
 */
binn *serialize_block_w_hash(block *input);

/**
 * @brief Deserializes a block by creating a new block (using calloc) and 
 *  initializing its attributes using those stored in the binn object passed 
 *  into the method.
 *
 * @param b The binn obj (block) that must be deserialized.
 *
 * @return The new block that contains the attributes that were stored in binn
 *  object b.
 */
block *deserialize_block(binn *b);

/**
 * @brief Hashes a given block by serializing it, assigning it a random hash 
 *  and then freeing the serialized object. The new hash is created using a 
 *  malloc.
 *
 * @param b The block to be hashed.
 *
 * @return The new hash.
 */
hash *hash_block(block *b);

/**
 * TODO
 * 
 */
bool is_valid_block(block *b);

/**
 * @brief Acts as a tostring function, converting a block to a series of 
 *  comprehensible and user friendly strings.
 *
 *  @param b The block that must be printed.
 *
 *  @return The printable version of the block.
 */
char *to_string_block(block *b);

/**
 * @brief Prints the block and its specifications/attributes in a way that is
 *  useful to the user.
 *
 * @param b The block that must be printed.
 */
void print_block(block *b);

#endif
