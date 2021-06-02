#ifndef TOKENS_H
#define TOKENS_H

#include "../global_helpers/types.h"
#include <stdint.h>
#include <stdlib.h>

struct opcode_data_t {
  enum instr_type type;
  char *opcode;
};

union token_data {
  struct opcode_data_t opcode_data;
  uint8_t reg;
  uint32_t exp;
  uint8_t shift_name;
  uint8_t cond;
  char *address;
  char *error;
};

// TODO: break down address where?
enum token_type { OPCODE, REG, EXPRESSION, SHIFTNAME, COND, ADDRESS, TOKERR };

typedef struct token {
  union token_data data;
  enum token_type type;
} token_t;

#endif /* TOKENS_H*/
