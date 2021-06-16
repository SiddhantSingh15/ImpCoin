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
  uint16_t amount;
} transaction_msg;

#endif // MESSAGES_H