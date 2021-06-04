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
 * @param instr_array pointer to the instruction array.
 * @param array_size size of the instruction array.
 * @param save_name name of the new binary file to be made.
 */
void write_file(uint32_t *instr_array, int array_size, char *save_name);

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
