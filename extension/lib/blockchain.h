#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <time.h>
#include <pthread.h>

#define GENESIS_BLOCK genesis_block()

/**
 * @brief The struct for a blockchain that stores a series of blocks as well as
 *  a linked list for the mempool/list of pending transactions.
 */
typedef struct blockchain {
  block *latest_block;
  linked_list *mempool;
} blockchain;

/**
 * @brief Creates (mallocs) and initializes a new blockchain using the 
 *  GENESIS_BLOCK and a new mempool linked list.
 *
 * @return The new blockchain.
 */
blockchain *init_blockchain(void);

/**
 * @brief Frees the memory allocated to a given blockchain.
 *
 * @param chain The blockchain to be freed.
 */
void free_blockchain(blockchain *chain);

/**
 * @brief Creates and initializes a memory pool (linked list of transactions) 
 *  to be stored in the blockchain.
 *
 *  @return The new empty linked list.
 */
linked_list *init_mempool(void);

/**
 * @brief Frees the memory space allocated to the mempool stored in the 
 *  blockchain, including all the nodes and its' values in the linked list.
 */
void free_mempool(linked_list *mempool);

/**
 * @brief Initializes and returns the genesis block/initial block that is 
 *  always a constant in the blockchain. It is the first node/root of the
 *  blockchain.
 *
 * @return The genesis block.
 */
block *genesis_block(void);

/**
 * @brief Appends the given block at the end of the given blockchain.
 *
 * @param chain The blockchain to which the block must be appended.
 * @param b The block that must be appended to the blockchain.
 *
 * @return Returns true if the appendage was successful and false otherwise.
 */
bool append_to_blockchain(blockchain *chain, block *b);

/**
 * @brief Traverses a blockchain to find and return the block at a specific 
 *  index.
 *
 * @param chain The blockchain where the block to be found lies.
 * @param block_num The index/block number of the block that must be returned.
 *
 * @return The block at the specified index.
 */
block *traverse_blockchain(blockchain *chain, uint32_t block_num);

/**
 * @brief Serializes the given blockchain by creating a new binn object that
 *  stores all the properties of the blockchain, including the serialized 
 *  versions of its blocks.
 *
 * @param bc The blockchain to be serialized.
 *
 * @return The serialized blockchain as a binn object.
 */
binn *serialize_blockchain(blockchain *bc);

/**
 * @brief Takes the binn object of a blockchain and deserializes it by creating
 *  (malloc-ing) a new blockchain to store all the properties of the binn
 *   object.
 *
 * @param input The binn object to be deserialized.
 * 
 * @return The deserialized blockchain.
 */
blockchain *deserialize_blockchain(binn *input);

/** 
 * @brief Creates a new block 
 *
 * @param bc The pointer to the blockchain.
 * @param username The username of the user making the new block.
 *
 * @return Returns the new block created in the new blockchain.
 */
block *new_block(blockchain *bc, const char *username);

/**
 * @brief
 *
 * @param bc
 * @param username
 * @param mutex
 *
 * @return
 */
block *proof_of_work(blockchain *bc, const char *username, pthread_mutex_t *mutex);

/**
 * @brief Returns the balance of a specific user in the blockchain given a
 *  username.
 *
 * @param bc The blockchain to be searched for the user.
 * @param username The username of the user whose balance is to be returned.
 *
 * @return The balance of the specified user.
 */
uint64_t get_balance(blockchain *bc, const char *username);

/**
 * @brief Checks if the given current blockchain is valid by ensuring the 
 *        previous hash of the block matches with the hash of the previous 
 *        block. This is done by traversing the linked list. The index of the 
 *        the blocks are also checked (to make sure theyre in the correct order)
 *
 * @param curr The current blockchain.
 * @param incoming The blockchain with the latest block.
 *
 * @return Returns true if the blockchain is valid and false otherwise.
 */
bool blockchain_valid (blockchain *curr, blockchain *incoming);

/**
 * @brief Prints the given blockchain and its properties/attributes.
 *
 * @param chain The blockchain to be printed.
 */
void print_blockchain(blockchain *chain);

/**
 * @brief Converts the blockchain to a series of user-friendly strings that 
 *  contain the values contained as its attributes.
 *
 * @param chain The blockchain that must be converted to string.
 *
 * @return The string value produced after converting to string.
 */
char *blockchain_to_string(blockchain *chain);


#endif
