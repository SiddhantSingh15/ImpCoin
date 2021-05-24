#ifndef SRC_IOUTILS_H
#define SRC_IOUTILS_H

#include <stdio.h>

void read_file(char* filename, arm11_state_t* state);
void print_size(void);
arm11_state_t *init_state();
void free_state_memory(arm11_state_t *state);
void print_arm11_state(arm11_state_t *state);

#endif
