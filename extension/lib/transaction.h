#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <time.h>
#include <stdint.h>

typedef struct transaction {
  time_t timestamp;
  char to[32];
  char from[32];
  uint64_t amount;
} transaction;

#endif
