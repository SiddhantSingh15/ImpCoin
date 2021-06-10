#ifndef ASSEMBLE_UTILS_H
#define ASSEMBLE_UTILS_H

#include "../global_helpers/definitions.h"
#include "linked_list.h"
#include "symbol_table_utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Saves the instruction array into the specified binary file.
 *
 * @param file File to write to.
 * @param binary_instr the 32 bit binary instruction to write.
 */
void write_file(FILE *file, uint32_t *binary_instr);

/**
 * @brief Reads the given file and converts each line into a list of tokens and
 * puts them into a linked list.
 *
 * @param filename The file to be read.
 * @param instruction The linked list to which tokens must be added.
 * @param symbols The table where we add labels-adress path.
 */
void read_asm(char *filename, linked_list *instructions, symbol_table *symbols);

/**
 * @brief Sets up the symbol table to contain instruction names. Each
 * instruction name maps to a code and its parse function.
 *
 * @param st The symbol table that we are adding to.
 */
void populate_st_instructions(symbol_table *st);

/**
 * @brief Checks if the token is a shift type.
 *
 * @param token The token to check.
 *
 * @return returns the relevant int8_t of the shift type from definitions.h.
 */
int8_t is_shift(char *token);

#endif /* ASSEMBLUE_UTILS_H */
