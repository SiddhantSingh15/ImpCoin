#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <time.h>
#include <stdint.h>

#define UID_LENGTH 32

/**
 * @brief A struct to store a certain transaction that occurs between accounts.
 *  It stores the account from which transaction occurs, the account to which
 *  transaction is occuring and the amount that is transferred between
 *  accounts.
 */
typedef struct transaction {
  time_t timestamp;
  char to[UID_LENGTH];
  char from[UID_LENGTH];
  uint64_t amount;
} transaction;

/**
 * @brief Creates a new transaction (using calloc) and initializes its
 *  attributes using the parameters passed into the method.
 *
 * @param from The username of the account from which transaction occurs.
 * @param to The username of the account to which transaction occurs.
 * @param amount The amount exchanged during transaction.
 * @param time The time at which transaction occurs.
 *
 * @return The new transaction.
 */
transaction *init_transaction(char *from, char *to, uint64_t amount,
  time_t time);

/**
 * @brief Returns a duplicate of the given transaction.
 *
 * @param t The transaction to be duplicated.
 *
 * @return The new duplicated transaction.
 */
transaction *dup_transaction(transaction *t);

/**
 * @brief Returns the serialized version (binn object) of the given
 *  transaction.
 *
 * @param t The transaction to be serialized.
 *
 * @return The new binn object (serialized transaction).
 */
binn *serialize_transaction(transaction *t);

/**
 * @brief Returns a binn object containing a list of serialized transactions,
 *  given a list of transactions to serialize.
 *
 * @param ts The linked list of transactions to be serialized.
 *
 * @return The binn object (serialized version) of the given linked list of
 *  transactions.
 */
binn *serialize_transactions(linked_list *ts);

/**
 * @brief Deserializes and returns a transaction, given the binn object of that
 *  transaction.
 *
 * @param trn The (binn object) serialized transaction to be deserialized.
 *
 * @return The deserialized transaction.
 */
transaction *deserialize_transaction(binn *trn);

/**
 * @brief Deserializes a given serialized list of transactions.
 *
 * @param transactions The binn object of (serialized) list of transactions.
 *
 * @return The deserialized list of transactions.
 */
linked_list *deserialize_transactions(binn *transactions);

/**
 * @brief Checks if a transaction is valid and returns true if so.
 *
 * @param tc The transaction to be checked for validity.
 * @param bc_ptr The blockchain to check the transaction against.
 *
 * @return True if the transaction is valid and false otherwise.
 */
bool is_valid_transaction(transaction *tc, void *bc_ptr);

/**
 * @brief Converts a given transaction to readable form as a string that
 *  contains the attributes of the transaction in a comprehensible way.
 *
 * @param t The transaction to be converted to string.
 * @param bug The string to store the converted string.
 */
void to_string_transaction(void *t, char *buf);

/**
 * @brief Prints the given transaction and its attributes in a readable form
 *  that is useful to the user.
 *
 * @param t The transaction to be printed.
 */
void print_transaction(transaction *t);

/**
 * @brief Frees the memory space allocated to a given transaction.
 *
 * @param t The transaction to be freed.
 */
void free_transaction(void *t);

#endif
