#include <stdio.h>
#include <stdlib.h>
#include "assemble_lib/assemble_utils.h"
#include "assemble_lib/linked_list.h"

int main(int argc, char **argv) {

  if (argc != 2) {
    printf("Please provide only 1 argument\n");
    return EXIT_FAILURE;
  }
  char *file = argv[1];

  linked_list *instructions = init_linked_list();
  symbol_table *labels = init_symbol_table();

  read_asm(file, instructions, labels);

  // loop through the instructions (token_list) and decipher them
  // loop through the instructions (uint32_t) and write them

  return EXIT_SUCCESS;
}
