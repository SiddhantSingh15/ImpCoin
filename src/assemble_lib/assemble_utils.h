#ifndef ASSEMBLE_UTILS_H
#define ASSEMBLE_UTILS_H

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
