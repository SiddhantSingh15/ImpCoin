#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>
#include <stdbool.h>
#include "../definitions.h"

/**
 * @brief A struct to store metadata/message of a blockchain. It stores the 
 *  message type, username of the account and the blockchain it belongs to.
 */
typedef struct {
  char type[MESSAGE_TYPE_SIZE];
  char username[UID_LENGTH];
  blockchain *bc;
} blockchain_msg;

/**
 * @brief A struct to store a transaction message. It contains the message 
 *  type, the username of the account, the account to which the transaction is
 *  occuring, the amount transferred in the transaction and the time at which
 *  the transaction occurs.
 */
typedef struct {
  char type[MESSAGE_TYPE_SIZE];
  char username[UID_LENGTH];
  char to[UID_LENGTH];
  uint64_t amount;
  time_t timestamp;
} transaction_msg;

/**
 * @brief Serializes the blockchain along with any metadata into a binn object
 *  of blockchain message.
 * 
 * @param bc_msg The blockchain message to be serialized.
 *
 * @return The binn object created after serializing the blockchain message.
 */ 
binn *serialize_bc_msg(blockchain_msg *bc_msg);

/**
 * @brief Deserializes the blockchain message into the blockchain struct and 
 *  its metadata.
 *
 * @param input The binn object of the blockchain message that is to be 
 *  deserialized.
 *
 * @return The deserialized blockchain message.
 */
blockchain_msg *deserialize_bc_msg(binn *input);

/**
 * @brief Serializes the given transaction message to a binn object.
 *
 * @param t_msg The transaction message to be serialized.
 *
 * @return The binn object of serialized transaction message.
 */
binn *serialize_t_msg(transaction_msg *t_msg);

/**
 * @brief deserializes the given binn object of a transaction message.
 *
 * @param input the binn object (transaction message) to be deserialized.
 *
 * @return The deserialized transaction message.
 */
transaction_msg *deserialize_t_msg(binn *input);

/**
 * @brief Takes a transaction message and converts it to a transaction type
 *  struct by creating and initializing a transaction with the relevant 
 *  attributes contained in the tansaction message.
 *
 * @param tc_msg The transaction message that must be converted to a 
 *  transaction.
 *
 * @return The new transaction.
 */
transaction *to_transaction(transaction_msg *tc_msg);

/**
 * @brief Creates and initialized a new transaction message, given the amount
 *  transferred, the username of the account from which the tranfer occurs and
 *  the account to which it occurs.
 *
 * @param amount The amount transferred.
 * @param username The username of the account from which transfer occurs.
 * @param to The user to which the transfer message is sent.
 *
 * @return The new created transaction message.
 */
transaction_msg *init_transaction_msg(uint64 amount, const char *username,
                                      char *to);
#endif // MESSAGES_H
