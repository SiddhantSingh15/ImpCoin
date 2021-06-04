#ifndef ASSEMBLE_UTILS_H
#define ASSEMBLE_UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "symbol_table_utils.h"
#include "linked_list.h"


/**
 * @brief Saves the instruction array into the specified binary file.
 *
 * @param file File to write to.
 * @param binary_instr the 32 bit binary instruction to write.
 */
void write_file(FILE *file, uint32_t *binary_instr);

void read_asm(char *filename, linked_list *instructions, symbol_table *symbols);

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
