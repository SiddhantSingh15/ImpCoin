#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "global_helpers/types.h"
#include "emulate_lib/decoder.h"
#include "emulate_lib/emulate_utils.h"
#include "emulate_lib/exec_instructions.h"

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
      execute(state->pipeline->executed, state);
      free(state->pipeline->executed);
    }

    state->pipeline->decoded = state->pipeline->fetched;
    // Decode an instruction
    if (state->pipeline->decoded != NULL) {
      state->pipeline->executed = decode(state->pipeline->decoded);
    }

    // Fetch an instruction
    fetch_next(state);

    // Increment the PC
    state->register_file[PC] += 4;
  }


  // Once everything is done, free state memory and pipeline
  print_arm11_state(state);
  flush_pipeline(state->pipeline);
  free_state_memory(state);
}
