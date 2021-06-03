#ifndef TOKENS_H
#define TOKENS_H

#include "../global_helpers/types.h"
#include <stdint.h>
#include <stdlib.h>

union token_data {
  char *instr_name;
  uint8_t reg;
  uint32_t exp;
  uint8_t shift_name;
  uint8_t cond;
  char separator;
  char *label;
  char *error;
};

enum token_type {
  INSTRNAME,
  REG,
  EXPRESSION,
  SHIFTNAME,
  COND,
  SEPARATOR,
  LABEL,
  TOKERR
};

typedef struct token {
  union token_data data;
  enum token_type type;
} token_t;

typedef struct token_list {
  struct token list[16];
  uint8_t size;
} token_list;

#endif /* TOKENS_H */
