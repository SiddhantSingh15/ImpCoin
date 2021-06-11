#include "../global_helpers/definitions.h"
#include "../global_helpers/types.h"
#include "tokens.h"
#include "tokenizer.h"
#include "linked_list.h"
#include "symbol_table.h"
#include "assemble_utils.h"
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

token_list *tokenizer(char *instr_line) {
  // pre: instr_line is not a label. It is a complete instruction string

  token_list *tokens = calloc(1, sizeof(token_list));
  PTR_CHECK(tokens, "Memory allocation failure\n");

  uint8_t count = 0;
  char *token;
  char *rest_start = malloc(strlen(instr_line) + 1);
  PTR_CHECK(rest_start, "Memory allocation failure\n");
  strcpy(rest_start, instr_line);

  // A second variable is needed to store the pointer to the beginning of the
  // string. The pointer 'rest' will be changed by str_brk to point to other
  // parts of the string later in str_brk
  char *rest = rest_start;

  // Before analyzing the instruction, we can extract the instruction name first
  // It will always be the leftmost word in the string
  char *instr_name = malloc(strlen(instr_line + 1));
  PTR_CHECK(instr_name, "Memory allocation failure\n");
  token = strbrk_r(rest, BLANK_SPACE, &rest);
  strcpy(instr_name, token);
  tokens->list[count].type = INSTRNAME;
  tokens->list[count].data.instr_name = instr_name;
  count++;
  free(token);

  // Convert the remaining string into an array of tokens
  while ((token = strbrk_r(rest, DELIMITERS, &rest))) {

    enum token_type type;
    union token_data data;

    if (strcspn(token, COMMA_SPACE) == 0) {
      free(token);
      continue;
    }

    if (isdigit(*token)) {
      type = EXPRESSION;
      data.exp = parse_int(token);

    } else if (*token == REGISTER_SYMB && isdigit(token[1])) {
      type = REG;
      data.reg = (uint8_t)parse_int(token + 1);

    } else if (is_shift(token) >= 0) {
      type = SHIFTNAME;
      data.shift_name = is_shift(token);

    } else {
      // Malloc a new token since these cases store pointers instead of values
      char *token_dupe = malloc(strlen(token) + 1);
      PTR_CHECK(token_dupe, "Memory allocation failure\n");
      strcpy(token_dupe, token);
      if (strlen(token) == 1) {
        type = SEPARATOR;
        data.separator = *token_dupe;

      } else if (isalpha(*token)) {
        type = LABEL;
        data.label = token_dupe;

      } else {
        type = TOKERR;
        data.error = token_dupe;
      }
    }

    tokens->list[count].type = type;
    tokens->list[count].data = data;
    free(token);
    count++;
  }
  free(rest_start);
  tokens->size = count;
  return tokens;
}

/*
  s      = "[r1, r2]"
  delims = "[], "
  save_pointer = &s

  token 1: [        0       rest: r1, r2]
  token 2: r1       2       rest: , r2]
  token 3: ,        0       rest:  r2]
  token 4:          0       rest: r2]
  token 5: r2       2       rest: ]
  token 6: ]        0       rest:
*/
char *strbrk_r(char *s, const char *delims, char **save_pointer) {

  if (s == NULL) {
    s = *save_pointer;
  }

  if (*s == LINE_TERMINATOR) {
    return NULL;
  }

  char *token = malloc(sizeof(s));
  PTR_CHECK(token, "Memory allocation failure\n");
  int tok_size = strcspn(s, delims);
  tok_size = tok_size == 0 ? 1 : tok_size;

  assert(tok_size <= strlen(s));
  *token = LINE_TERMINATOR;
  strncat(token, s, tok_size);
  token = realloc(token, sizeof(char) * (strlen(token) + 1));
  PTR_CHECK(token, "Memory allocation failure\n");

  *save_pointer += tok_size;

  return token;
}

uint32_t parse_int(char *str) {
  int base = DENARY;

  if (strlen(str) == 1) {
    return strtol(str, NULL, base);
  }

  switch (str[1]) {
  case 'x':
    base = HEX;
    break;
  case 'b':
    base = BINARY;
    break;
  case 'o':
    base = OCTAL;
    break;
  default:
    base = DENARY;
  }

  return strtol(str, NULL, base);
}
