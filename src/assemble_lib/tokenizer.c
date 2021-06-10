#include "../global_helpers/definitions.h"
#include "tokenizer.h"
#include "assemble_utils.h"
#include "tokens.h"
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

token_list *tokenizer(char *instr_line) {
  // pre: instr_line is not a label, only an instruction

  token_list *tokens = calloc(1, sizeof(token_list));

  uint8_t count = 0;
  char *token;
  char *rest_start = malloc(strlen(instr_line) + 1);
  strcpy(rest_start, instr_line);
  char *rest = rest_start;

  char *instr_name = malloc(strlen(instr_line + 1));
  strcpy(instr_name, strbrk_r(rest, BLANK_SPACE, &rest));
  tokens->list[count].type = INSTRNAME;
  tokens->list[count].data.instr_name = instr_name;
  count++;

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
      char *token_dupe = malloc(strlen(token) + 1);
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
  int tok_size = strcspn(s, delims);
  tok_size = tok_size == 0 ? 1 : tok_size;

  assert(tok_size <= strlen(s));
  *token = LINE_TERMINATOR;
  strncat(token, s, tok_size);
  token = realloc(token, sizeof(char) * (strlen(token) + 1));

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
