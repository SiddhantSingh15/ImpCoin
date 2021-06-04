#include "linked_list.h"
#include "symbol_table_utils.h"
#include "tokenizer.h"
#include "tokens.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../global_helpers/definitions.h"

void write_file(uint32_t *instr_array, int array_size, char *save_name) {
  FILE *file = fopen(save_name, "wb");

  for (int i = 0; i < array_size; i++) {
    fwrite(&instr_array[i], INSTR_SIZE, 1, file);
  }

  fclose(file);
}

// reads the assembly file and populates the linked list and symbol table
void read_asm(char *filename, linked_list *instructions,
              symbol_table *symbols) {

  FILE *fptr;
  char *buffer = NULL;
  size_t bufsize = 511;
  uint32_t mem_address;

  if ((fptr = fopen(filename, "r")) == NULL) {
    printf("Error opening file.\n");
    exit(EXIT_FAILURE);
  }

  while (getline(&buffer, &bufsize, fptr)) {

    if (ferror(fptr)) {
      perror("Error occurred when reading file\n");
      exit(EXIT_FAILURE);
    }

    // remove trailing newline
    buffer[strcspn(buffer, "\n")] = '\0';

    if (buffer[strlen(buffer) - 1] == ':') {
      // it is a label, add to symbol table
      // TODO: add to symbol table
      continue;
    }

    // tokenize, and add to the linked list
    token_list *tokens = tokenizer(buffer);
    append_to_linked_list(instructions, tokens, mem_address);

    mem_address += 4;
  }

  free(buffer);
  buffer = NULL;

  fclose(fptr);
}
