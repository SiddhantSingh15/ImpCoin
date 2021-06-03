#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tokens.h"
#include "tokenizer.h"

token_list *tokenizer(char *instr_line) {
  // pre: instr_line is not a label, only an instruction

  token_list *tokens = calloc(1, sizeof(token_list));

  uint8_t count = 0;
  // char *token;
  char *rest = strdup(instr_line);

  char *instr_name = strtok_r(rest, " ", &rest);
  tokens->list[count].type = INSTRNAME;
  tokens->list[count].data.instr_name = instr_name;
  count++;

  /*
  while ((token = strtok2(rest, ",[]#=", &rest))) {
    if (token[0] == ',' || token[0] == ' ') {
      continue;
    }
    // split separators
    // figure out token type
    // read associated data
    // create and assign token
  }
  */

  return NULL;
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
