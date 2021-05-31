#ifndef SYMBOL_TABLE_UTILS_H
#define SYMBOL_TABLE_UTILS_H

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
 * @param header The symbol table pointer
 * @param label The string of the label
 * @param address The address associated with the label
 */
void symbol_table_insert (symbol_table_kvp *header, char* label, int address);

#endif // SYMBOL_TABLE_UTILS_H