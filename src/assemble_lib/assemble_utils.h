#ifndef ASSEMBLE_UTILS_H
#define ASSEMBLE_UTILS_H

#include <stdint.h>
#include <stdlib.h>


/**
 * @brief Saves the instruction array into the specified binary file.
 * 
 * @param instr_array pointer to the instruction array.
 * @param array_size size of the instruction array.
 * @param save_name name of the new binary file to be made.
 */
void write_file(uint32_t *instr_array, int array_size, char *save_name);

#endif /* ASSEMBLUE_UTILS_H */