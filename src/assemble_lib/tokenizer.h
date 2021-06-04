#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "tokens.h"
#include "tokenizer.h"
#include "assemble_utils.h"

token_list *tokenizer(char *instr_line);

char *strbrk_r(char *s, const char *delims, char **save_pointer);

uint32_t parse_int(char *string);

#endif /* TOKENIZER_H */
