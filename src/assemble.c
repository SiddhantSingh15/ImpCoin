#include "global_helpers/definitions.h"
#include "global_helpers/types.h"
#include "assemble_lib/linked_list.h"
#include "assemble_lib/symbol_table.h"
#include "assemble_lib/assemble_utils.h"
#include "assemble_lib/tokens.h"
#include "assemble_lib/tokenizer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// reads the assembly file and populates the linked list and symbol table
void read_asm(char *filename, linked_list *instructions,
              symbol_table *symbols) {

  FILE *fptr;
  char buffer[INSTR_BUFFER];
  uint32_t mem_address = 0;
  uint32_t buffer_len;

  if ((fptr = fopen(filename, "r")) == NULL) {
    printf("Error opening file.\n");
    exit(EXIT_FAILURE);
  }
  // First read
  // - Add labels into symbol table
  // - Convert each instruction into array of tokens
  while (fgets(buffer, INSTR_BUFFER, fptr)) {
    // remove trailing newline
    buffer[strcspn(buffer, "\n")] = '\0';
    // get length of buffer without newline
    buffer_len = strlen(buffer);

    // If source file has gaps in program, skip that line
    if (buffer_len == 0) {
      continue;
    }

    if (buffer[buffer_len - 1] == ':') {
      // it is a label, add it to the symbol table
      // replace ':' with terminating character
      buffer[buffer_len - 1] = '\0';
      st_insert(symbols, buffer, &mem_address, sizeof(uint32_t));
      continue;
    }

    // tokenize, and add it to the instruction list
    token_list *tokens = tokenizer(buffer);
    PTR_CHECK(tokens, "Error in tokenizer\n");
    // append_to_linked_list returns a memory that should be equal to 
    // mem_address 
    if (append_to_linked_list(instructions, tokens) != mem_address){
      perror("Error appending to linked list\n");
      exit(EXIT_FAILURE);
    }

    mem_address += WORD_SIZE_IN_BYTES;
  }

  if (ferror(fptr)) {
    perror("Error occurred when reading file\n");
    exit(EXIT_FAILURE);
  }

  fclose(fptr);
}

int main(int argc, char **argv) {

  if (argc != 3) {
    printf("Please provide only 2 arguments\n");
    return EXIT_FAILURE;
  }
  char *file = argv[1];

  linked_list *instructions = init_linked_list();
  symbol_table *labels = init_symbol_table();
  populate_st_instructions(labels);

  read_asm(file, instructions, labels);

  FILE *to_write = fopen(argv[2], "w");

  // loop through the instructions (token_list) and decipher them
  // loop through the instructions (uint32_t) and write them

  /* Comes inside the FOR loop */
  token_list *tokens;
  node *curr = instructions->head;
  instr_func_map *func_map;
  uint32_t binary_instr;
  while (curr != NULL) {
    binary_instr = 0;
    tokens = curr->value;
    if (tokens->list[0].type == INSTRNAME) {
      // curr->value is a token_list, and the first token has been correctly
      // initialised to type INSTRNAME.
      func_map = st_retrieve(labels, tokens->list[0].data.instr_name);
      PTR_CHECK(func_map, "Value does not exist in symbol table\n");
      binary_instr = func_map->function(curr, func_map->code, labels);
    }
    // curr-> value is not a token_list, but a constant binary value.
    else {
      if (tokens->list[0].type != EXPRESSION) {
        perror("Invalid leading token.\n");
        exit(EXIT_FAILURE);
      }
      binary_instr = tokens->list[0].data.exp;
    }

    curr = curr->next;
    write_file(to_write, &binary_instr);
  }
  fclose(to_write);
  // Free linked list
  free_linked_list(instructions, free_token_list);
  // Free symbol table
  free_symbol_table(labels);
  return EXIT_SUCCESS;
}
