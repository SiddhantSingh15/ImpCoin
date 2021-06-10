#include "../global_helpers/definitions.h"
#include "../global_helpers/types.h"
#include "symbol_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

symbol_table *init_symbol_table(void) {
  symbol_table *st = calloc(1, sizeof(symbol_table));
  PTR_CHECK(st,"Memory allocation failure\n");
  st->capacity = TABLE_CAPACITY;
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
 uint32_t hash (char* key) {
  uint32_t acc = 0;
  size_t index = 0;
  while (key[index] != '\0') {
    acc += key[index] >> 2;
    index++;
  }
  return acc % TABLE_CAPACITY;
}

uint32_t rehash(uint32_t prev_hash) {
  return (prev_hash + 1) % TABLE_CAPACITY;
}

void *st_retrieve (symbol_table *st, char* key) {
  uint32_t hashcode = hash(key);
  while (st->kvps[hashcode] != NULL && strcmp(st->kvps[hashcode]->key, key)) {
    hashcode = rehash(hashcode);
  }
  if (st->kvps[hashcode] == NULL) {
    return NULL;
  }
  return st->kvps[hashcode]->value;
}

void st_insert (symbol_table *st, char* key, void* value, uint32_t vsize) {
  if (st->count == st->capacity) {
    fprintf(stderr, "Symbol Table is at full capacity.\n");
    exit(EXIT_FAILURE);
  }

  void *st_value = malloc(vsize);
  PTR_CHECK(st_value, "Memory allocation failure\n");
  memcpy(st_value, value, vsize);

  // Create the key-value pair
  symbol_table_kvp *kvp = calloc(1, sizeof(symbol_table_kvp));
  char* new_key = malloc(strlen(key) + 1);
  PTR_CHECK(new_key, "Memory allocation failure\n");
  strcpy(new_key, key);
  kvp->key = new_key;
  kvp->value = st_value;

  // Apply hash function
  uint32_t hashcode = hash(key);

  // Insert into hash table
  if (st->kvps[hashcode] == NULL) {
    // Hashcode unoccupied
    st->count++;
  } else {
    // Hashcode occupied
    // Two keys of the same name will never be added in
    if (!strcmp(key,st->kvps[hashcode]->key)) {
      fprintf(stderr, "Two keys of the same name are being inserted.\n");
      exit(EXIT_FAILURE);   
    }
    // Encounter collision
    // Repeatedly attempt to rehash until an available NULL position is found
    do {
      hashcode = rehash(hashcode);
    } while (st->kvps[hashcode] != NULL);
  }
  st->kvps[hashcode] = kvp;
}

void free_symbol_table (symbol_table *st) {
  for (int i = 0; i < TABLE_CAPACITY; i++) {
    if (st->kvps[i] != NULL){
      free(st->kvps[i]->key);
      free(st->kvps[i]->value);
      free(st->kvps[i]);
    }
  }
  free(st);
}
