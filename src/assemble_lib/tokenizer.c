#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tokens.h"
#include "tokenizer.h"
#include "assemble_utils.h"

token_list *tokenizer(char *instr_line) {
  // pre: instr_line is not a label, only an instruction

  token_list *tokens = calloc(1, sizeof(token_list));

  uint8_t count = 0;
  char *token;
  char *rest = strdup(instr_line);

  char *instr_name = strtok_r(rest, " ", &rest);
  tokens->list[count].type = INSTRNAME;
  tokens->list[count].data.instr_name = instr_name;
  count++;

  while ((token = strbrk_r(rest, ",[]#=+-", &rest))) {

    enum token_type type;
    union token_data data;

    if (strcspn(token, ",") == 0) {
      continue;
    }

    if (strlen(token) == 1) {
      type = SEPARATOR;
      data.separator = *token;

    } else if (*token == 'r' && isdigit(token[1])) {
      type = REG;
      data.reg = (uint8_t) parse_int(token + 1);

    } else if (isdigit(*token)) {
      type = EXPRESSION;
      data.exp = parse_int(token);

    } else if (is_shift(*token) >= 0) {
      type = SHIFTNAME;
      data.shift_name = is_shift(*token);

    } else if () {
      type = LABEL;
      data.label = ;

    } else if () {
      type = TOKERR;
      data.error = ;
    }

    // TODO: if token is shiftname, initialize shiftname
    // shiftname is always strlen() == 3

    // TODO: if token is cond, initialize cond
    // cond is always strlen() == 2

    // TODO: how to distinguish between label and invalid token?
    // label is apparently not always alphabetic

    free(token);
    tokens->list[count].type = type;
    tokens->list[count].data = data;
    count++;
  }

  free(rest);
  return tokens;
}

/*
  // s      = "[r1, r2]"
  // delims = "[], "
  // save_pointer = &s
  //
  // token 1: [        0       rest: r1, r2]
  // token 2: r1       2       rest: , r2]
  // token 3: ,        0       rest:  r2]
  // token 4:          0       rest: r2]
  // token 5: r2       2       rest: ]
  // token 6: ]        0       rest:
*/
char *strbrk_r(char *s, const char *delims, char **save_pointer) {

  if (s == NULL) {
    s = *save_pointer;
  }

  if (*s == '\0') {
    return NULL;
  }

  char *token = malloc(sizeof(s));
  int tok_size = strcspn(s, delims);
  tok_size = tok_size == 0 ? 1 : tok_size;

  assert(tok_size <= strlen(s));
  *token = '\0';
  strncat(token, s, tok_size);
  token = realloc(token, sizeof(char) * strlen(token));

  *save_pointer += tok_size;

  return token;
}

uint32_t parse_int(char *str) {
  // TODO: implement this
  // this should be able to parse hexadecimal numbers with `0x` prefix
  //
  // use strtol and cast to uint32_t

  int base = 10;
  
  if (strlen(str) == 1) {
    return strtol(str, NULL, base);
  }

  switch (str[1]) {
    case 'x':
      base = 16;
    case 'b':
      base = 2;
    case 'o':
      base = 8;
    default:
      base = 10;
  }

  return strtol(str, NULL, base);
}
