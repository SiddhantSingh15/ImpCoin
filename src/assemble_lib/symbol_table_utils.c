#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../global_helpers/types.h"

symbol_table *init_symbol_table() {
  symbol_table *st = calloc(1, sizeof(symbol_table));
  st->capacity = TABLE_CAPACITY;
  st->kvps = malloc(TABLE_CAPACITY * sizeof(symbol_table_kvp));
  for (int i = 0; i < TABLE_CAPACITY; i++) {
    st->kvps[i] = NULL;
  }
  return st;
}

/**
 * @brief A simple hash function used by the symbol table
 *        This function shifts the ASCII value of each character to the right
 *        by 2, adds them together, then gets the remainder
 *        based on the size of the symbol table
 */ 
uint64_t hash (char* label) {
  uint64_t acc = 0;
  size_t index = 0;
  while (label[index] != '\0') {
    acc += label[index] >> 2;
    index++;
  }
  return acc % TABLE_CAPACITY;
}

bool contains_key (symbol_table_kvp *header, char* key) {
  return false;
}
