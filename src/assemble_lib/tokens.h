#ifndef TOKENS_H
#define TOKENS_H

#include "../global_helpers/types.h"
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief The data held by a token.
 */
union token_data {
  char *instr_name; /* Instruction name */
  uint8_t reg; /* Register type */
  uint32_t exp; /* Expression type */
  uint8_t shift_name; /* Shift name */
  uint8_t cond; /* Condition codes */
  char separator; /* Seperator */
  char *label; /* Label type */
  char *error; /* Token error */
};

/**
 * @brief Enum for the type of token.
 */
enum token_type {
  INSTRNAME, /* Instruction name */
  REG, /* Register */
  EXPRESSION, /* Expression */
  SHIFTNAME, /* Shift name */
  COND, /* Condition codes */
  SEPARATOR, /* Expression type */
  LABEL, /* Label type */
  TOKERR /* Token error */
};

/**
 * @brief Struct for token containing the data and the type of token.
 */
typedef struct token {
  union token_data data;
  enum token_type type;
} token_t;

/**
 * @brief Struct for the list of tokens corresponding to a line of the .s file.
 */
typedef struct token_list {
  struct token list[16];
  uint8_t size;
} token_list;

#endif /* TOKENS_H */
