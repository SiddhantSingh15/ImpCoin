#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  char type[10];
  char username[40];
  blockchain *bc;
} blockchain_msg;

typedef struct {
  char type[10];
  char username[40];
  char to[10];
  uint64_t amount;
  uint64_t timestamp;
} transaction_msg;

typedef struct {
  char type[10];
  char username[40];
  char to[10];
  uint64_t amount;
} confirmation_msg;

binn *serialize_bc_msg(blockchain_msg *bc_msg);

blockchain_msg *deserialize_bc_msg(binn *input);

binn *serialize_t_msg(transaction_msg *t_msg);

transaction_msg *deserialize_t_msg(binn *input);

transaction *to_transaction(transaction_msg *tc_msg);

#endif // MESSAGES_H
