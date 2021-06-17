#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <time.h>
#include <stdint.h>

#define UID_LENGTH 32

typedef struct transaction {
  time_t timestamp;
  char to[UID_LENGTH];
  char from[UID_LENGTH];
  uint64_t amount;
} transaction;

transaction *init_transaction(char *from, char *to, uint64_t amount,
  time_t time);

transaction *dup_transaction(transaction *t);

binn *serialize_transaction(transaction *t);

binn *serialize_transactions(linked_list *ts);

transaction *deserialize_transaction(binn *trn);

linked_list *deserialize_transactions(binn *transactions);

bool is_valid_transaction(transaction *tc, void *bc_ptr);

void to_string_transaction(void *t, char *buf);

void print_transaction(transaction *t);

void free_transaction(void *t);

#endif
