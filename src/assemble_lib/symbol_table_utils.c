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
 uint32_t hash (char* label) {
  uint32_t acc = 0;
  size_t index = 0;
  while (label[index] != '\0') {
    acc += label[index] >> 2;
    index++;
  }
  return acc % TABLE_CAPACITY;
}

uint32_t rehash(uint32_t prev_hash) {
  return (prev_hash * prev_hash + 1) % TABLE_CAPACITY;
}

uint32_t *retrieve_address (symbol_table *st, char* label) {
  uint32_t hashcode = hash(label);
  while (st->kvps[hashcode] != NULL && strcmp(st->kvps[hashcode]->key, label)) {
    hashcode = rehash(hashcode);
  }
  if (st->kvps[hashcode] == NULL) {
    return NULL;
  }
  return &st->kvps[hashcode]->value;
}

void free_symbol_table (symbol_table *st) {
  for (int i = 0; i < TABLE_CAPACITY; i++) {
    if (st->kvps[i] != NULL){
      free(st->kvps[i]->key);
    }
  }
  free(st);
}

void insert_to_symbol_table (symbol_table *st, char* label, int address) {
  if (st->count == st->capacity) {
    fprintf(stderr, "Symbol Table is at full capacity.\n");
    exit(EXIT_FAILURE);
  }

  // Create the key-value pair
  symbol_table_kvp *kvp = calloc(1, sizeof(symbol_table_kvp));
  char* new_key = malloc(strlen(label) + 1);
  strcpy(new_key, label);
  kvp->key = new_key;
  kvp->value = address;

  // Apply hash function
  uint32_t hashcode = hash(label);

  // Insert into hash table
  if (st->kvps[hashcode] == NULL) {
    // Hashcode unoccupied
    st->count++;
  } else {
    // Hashcode occupied
    // Two labels of the same name will never be added in
    if (!strcmp(label,st->kvps[hashcode]->key)) {
      fprintf(stderr, "Two labels of the same name are being inserted.\n");
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
