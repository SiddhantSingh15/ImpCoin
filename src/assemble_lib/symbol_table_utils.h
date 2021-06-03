#ifndef SYMBOL_TABLE_UTILS_H
#define SYMBOL_TABLE_UTILS_H

#include <stdint.h>
#include "../global_helpers/types.h"

/**
 * @brief Initializes a header node for a symbol table.
 *        This node will have an empty key
 * 
 * @return The pointer to the symbol table
 */
symbol_table *init_symbol_table();

/**
 * @brief Insert a label and its associated memory address into the symbol table
 * 
 * @param st The pointer to the symbol table
 * @param key The key of the key-pair to be inserted
 * @param value The value of the key-pair to be inserted
 */
void st_insert (symbol_table *st, char* key, void *value);

/**
 * @brief Retrieves the address from a symbol table
 * 
 * @param st The pointer to the symbol table
 * @param key The key of the key-pair to retrieve
 * 
 * @result The address of the provided label
 */
void *st_retrieve (symbol_table *st, char* key);

/**
 * @brief Frees all allocated memory for the symbol table
 * 
 * @param st The pointer to the symbol table
 */
void free_symbol_table (symbol_table *st);

#endif // SYMBOL_TABLE_UTILS_H