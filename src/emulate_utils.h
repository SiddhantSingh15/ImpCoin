#ifndef SRC_IOUTILS_H
#define SRC_IOUTILS_H

#include "emulate.h"
#include <stdint.h>
#include <stdio.h>

// IO
void read_file(char *filename, arm11_state_t *state);
void print_size(void);
void print_arm11_state(arm11_state_t *state);

// Machine state and pipeline
arm11_state_t *init_state();
void free_state_memory(arm11_state_t *state);
void fetch_next(arm11_state_t *state);
void flush_pipeline(pipeline_t *pipeline);

// Unit conversion
uint32_t to_uint32(uint8_t byte_array[WORD_SIZE_IN_BYTES]);
void to_uint8_array(uint32_t word, uint8_t byte_array[WORD_SIZE_IN_BYTES]);

#endif
