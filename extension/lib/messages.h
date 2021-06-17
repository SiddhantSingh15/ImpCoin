#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>
#include <stdbool.h>
#include "../definitions.h"

typedef struct {
  char type[10];
  char username[UID_LENGTH];
  blockchain *bc;
} blockchain_msg;

typedef struct transaction_msg {
  char type[10];
  char username[UID_LENGTH];
  char to[UID_LENGTH];
  uint64_t amount;
  time_t timestamp;
} transaction_msg;

binn *serialize_bc_msg(blockchain_msg *bc_msg);

blockchain_msg *deserialize_bc_msg(binn *input);

transaction_msg *init_transaction_msg(uint64 amount, const char *username,
                                      char *to);

binn *serialize_t_msg(transaction_msg *t_msg);

transaction_msg *deserialize_t_msg(binn *input);

transaction *to_transaction(transaction_msg *tc_msg);

#endif // MESSAGES_H
