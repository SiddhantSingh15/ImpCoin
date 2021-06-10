#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "../global_helpers/definitions.h"
#include "assemble_utils.h"
#include "tokens.h"
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Breaks up the line read from .s files into tokens.
 *
 * @param instr_line Line from the .s file.
 *
 * @return List of tokens from the line.
 */
token_list *tokenizer(char *instr_line);

/**
 * @brief In-house string breaking function. strtok_r could not be used due to
 *        its lack of portability. strtok_r and strdup dont work on Mac OS.
 *        Another feature of this function is that it retains the delimiter in
 *        the resultant token.
 *
 * @param s The string to be tokenised.
 * @param delims String of delimiters.
 * @param save_pointer Pointer to the save destination.
 *
 * @return Resultant token from input string `s`.
 */
char *strbrk_r(char *s, const char *delims, char **save_pointer);

/**
 * @brief Converts strings containing numbers of different bases (2,8,10,16)
 *        to uint32_t using strtol.
 *
 * @param string Input string ("0xff", "0b1011", "12" etc.)
 *
 * @return Converted 32 bit unsigned integer.
 */
uint32_t parse_int(char *string);

#endif /* TOKENIZER_H */
