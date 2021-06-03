#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "tokens.h"

token_list *tokenizer(char *instr_line);

char *strbrk_r(char *s, const char *delims, char **save_pointer);

#endif /* TOKENIZER_H */
