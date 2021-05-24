#include "emulate.h"
#include <stdlib.h>
#include <stdio.h>
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
	
	arm11_state_t *start_state = init_state();

	read_file(file, start_state);
  
  // Once everything is done, free state memory
  free_state_memory(start_state);
}
