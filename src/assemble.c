#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "assemble_lib/symbol_table_utils.h"
#include "global_helpers/definitions.h"
#include "assemble_lib/assemble_utils.h"
#include "assemble_lib/linked_list.h"
#include "assemble_lib/tokens.h"

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

    } else {
      // curr-> value is not a token_list, but a constant binary value.
      binary_instr = *(uint32_t *)curr->value;

    }

    curr = curr->next;
    write_file(to_write, &binary_instr);
  }
  fclose(to_write);

  return EXIT_SUCCESS;
}
