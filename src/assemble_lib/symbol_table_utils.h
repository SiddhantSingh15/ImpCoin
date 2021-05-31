#ifndef SYMBOL_TABLE_UTILS_H
#define SYMBOL_TABLE_UTILS_H

#include <stdint.h>
#include "../global_helpers/types.h"

/**
 * @brief Initializes a header node for a symbol table.
 *        This node will have an empty key
 * 
 * @return The pointer to the header of the symbol table
 */
symbol_table *init_symbol_table();

/**
 * @brief Insert a label and its associated memory address into the symbol table
 * 
 * @param st The symbol table
 * @param label The string of the label
 * @param address The address associated with the label
 */
void insert_to_symbol_table (symbol_table *st, char* label, int address);

/**
 * @brief Retrieves the address from a symbol table
 * 
 * @param st The symbol table
 * @param label The label string
 * 
 * @result The address of the provided label
 */
uint32_t retrieve_address (symbol_table *st, char* label);

#endif // SYMBOL_TABLE_UTILS_H