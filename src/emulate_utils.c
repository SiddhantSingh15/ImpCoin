#include "emulate_utils.h"
#include "emulate.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 *
 * @param filename Name of file for reading
 * @param memory Pointer to leading element of our state's memory array
 */

void read_file(char *filename, arm11_state_t *state) {
  printf("Attempting to read file: %s\n", filename);
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

  *new_state = (arm11_state_t){.pipeline = pipeline};

  return new_state;
}

void free_state_memory(arm11_state_t *state) {
  free(state->pipeline);
  free(state);
}

void fetch_next(arm11_state_t *state) {
  instruction_t *fetched_instruction = malloc(sizeof(instruction_t));
  // Set the incoming 32 byte data to be all 0s
  uint32_t incoming = 0;
  int curr = (state->register_file)[PC];

  assert(curr < MEM_SIZE - 1);

  // Shift and insert the 4 pieces of data into curr 8 bytes at a time
  incoming |= (state->main_memory)[curr];
  for (int i = 1; i < REG_SIZE; i++) {
    incoming <<= ONE_B;
    incoming |= (state->main_memory)[curr + i];
  }
  // Set up the union data
  union instr_data incoming_instruction_data = {incoming};
  *fetched_instruction =
      (instruction_t){.data = incoming_instruction_data, .tag = RAW};
  // Insert into pipeline for fetched
  state->pipeline->fetched = fetched_instruction;
}

// Free all of pipeline, used for branch command
void flush_pipeline(pipeline_t *pipeline) {
  free(pipeline->fetched);
  free(pipeline->decoded);
  free(pipeline->executed);
  pipeline->fetched = NULL;
  pipeline->decoded = NULL;
  pipeline->executed = NULL;
}

void print_arm11_state(arm11_state_t *state) {

  /* Looping through 17 registers */
  printf("Registers: \n");
  for (int i = 0; i < NUM_GENERAL; i++) {
    printf("$%d  :          %d (0x%08x)\n", i, state->register_file[i],
           state->register_file[i]);
  }

  /* Printing PC and CPSR */
  printf("PC  :          %d (0x%08x)\n", state->register_file[PC],
         state->register_file[PC]);
  printf("CPSR  :          %d (0x%08x)\n", state->register_file[CPSR],
         state->register_file[CPSR]);

  /* Looping through non-0 locations */
  printf("Non-zero memory: \n");
  for (int i = 0; i < MEM_SIZE; i += 4) {
    uint32_t word = to_uint32(&state->main_memory[i]);
    if (word == 0) {
      printf("0x%08x: 0x%08x", i, word);
    }
  }
}

/*
 * EXPERIMENTAL PIECE OF CODE (maybe we could pass the ARM state
 * and check through that)
 *
 */
uint32_t to_uint32(uint8_t byte_array[WORD_SIZE_IN_BYTES]) {
  uint32_t word = 0;
  for (int i = 0; i < 4; i++) {
    word = (word << 8) | byte_array[i];
  }
  return word;
}

void to_uint8_array(uint32_t word, uint8_t byte_array[WORD_SIZE_IN_BYTES]) {
  uint32_t mask = 0xff;
  for (int i = 0; i < 4; i++) {
    byte_array[i] = (word & mask) >> (8 * i);
    mask <<= 8;
  }
}
