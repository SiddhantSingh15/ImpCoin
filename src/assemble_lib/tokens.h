#ifndef TOKENS_H
#define TOKENS_H

#include <stdint.h>
#include <stdlib.h>

typedef struct token {
	union token_data data;
	enum token_type type;
} token_t;

union token_data {

};

enum token_type { OPCODE, RD, RN, OPERAND2, RM, RS, EXPR };



#endif /* TOKENS_H*/