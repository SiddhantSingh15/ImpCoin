#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "emulate.h"

/**
 *
 * @param filename Name of file for reading
 * @param memory Pointer to leading element of our state's memory array
 */

void read_file(char* filename, arm11_state_t* state) {
  printf("Attempting to read file: %s\n",filename);
  FILE *fptr;
  if ((fptr = fopen(filename, "rb")) == NULL) {
    printf("Error opening file.\n");
    exit(EXIT_FAILURE);
  }

  fread(state->main_memory, 63356, 1, fptr);

  if (ferror(fptr)) {
    perror("Error occurred when reading file\n");
    exit(EXIT_FAILURE);
  } else {
    printf("File read success\n");
  }
  fclose(fptr);
}

void print_size(void) {
  printf("size of dataproc_t: %ld\n", sizeof(dataproc_t));
  printf("size of multiply_t: %ld\n", sizeof(multiply_t));
  printf("size of sdt_t: %ld\n", sizeof(sdt_t));
  printf("size of branch_t: %ld\n", sizeof(branch_t));
  printf("size of instruction_t: %ld\n", sizeof(instruction_t));
  printf("size of pipeline_t: %ld\n", sizeof(pipeline_t));
  printf("size of arm11_state_t: %ld\n", sizeof(arm11_state_t));
}

arm11_state_t *init_state() {
  arm11_state_t *new_state = calloc(1, sizeof(arm11_state_t));
  pipeline_t *pipeline = calloc(1, sizeof(pipeline_t));

  *new_state = (arm11_state_t) {
    .pipeline = pipeline,
  };

  return new_state;
}

void free_state_memory(arm11_state_t *state) {
  free(state->pipeline);
  free(state);
}