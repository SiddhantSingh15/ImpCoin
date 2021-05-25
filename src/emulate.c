#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "decoder.h"
#include "emulate.h"
#include "emulate_utils.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Please provide only 1 argument\n");
		return EXIT_FAILURE;
	}
	char *file = argv[1];

	// Create variable and set it to return of init_state,
	// initialise based on the spec (use malloc)
	// Anything thats a pointer needs to mallocated (start_state needs to be
	// mallocated)

	arm11_state_t *state = init_state();

	read_file(file, state);

  while (state->pipeline->executed == NULL || state->pipeline->executed->tag != HALT) {
    // Execute an instruction
    if (state->pipeline->executed != NULL) {
      // execute()
      free(state->pipeline->executed);
    }

    // Decode an instruction
    if (state->pipeline->decoded != NULL) {
      decode(state->pipeline->decoded);
    }
    state->pipeline->executed = state->pipeline->decoded;
    state->pipeline->decoded = state->pipeline->fetched;

    // Fetch an instruction
    fetch_next(state);

    // Increment the PC
    state->register_file[PC] += 4;
  }


  // Once everything is done, free state memory and pipeline
  flush_pipeline(state->pipeline);
  free_state_memory(state);
}
