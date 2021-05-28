#ifndef SRC_IOUTILS_H
#define SRC_IOUTILS_H

#include "emulate.h"
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Reads the file passed to the program.
 * 
 * @param filename Name of file for reading
 * @param memory Pointer to leading element of our state's memory array
 */
void read_file(char *filename, arm11_state_t *state);

/**
 * @brief Prints the sizes of the instruction types to terminal
 */
void print_size(void);

/**
 * @brief Prints the state of all the 17 registers and any non-zero memory.
 * 
 * @param state Pointer to the current ARM11 state.
 */
void print_arm11_state(arm11_state_t *state);



/**
 * @brief Initialises the ARM11 state before startup.
 * @return The initialised ARM11 state.
 */
arm11_state_t *init_state();

/**
 * @brief Flushes the pipeline and frees the memory allocated to the pipeline.
 */
void free_state_memory(arm11_state_t *state);

/**
 * @brief Fetches the next instruction in the ARM11 system.
 * 
 * @param state Takes the pointer to the current state of the ARM11 system.
 */
void fetch_next(arm11_state_t *state);

/** @brief Frees the whole pipeline, used in BRANCH command.
 * 
 * @param pipeline Takes the pointer to the current pipeline.
 */
void flush_pipeline(pipeline_t *pipeline);


/**
 * @brief Turns a uint8_t byte array to a suitable uint32_t type.
 * 
 * @param byte_array Takes uint8_t array.
 * 
 * @return The proper uint32_t register appropriate for registers.
 */
uint32_t to_uint32_reg(uint8_t byte_array[WORD_SIZE_IN_BYTES]);

/**
 * @brief Prints a uint8_t byte array as a uint32_t register.
 * 
 * @param byte_array The input byte array to print.
 * 
 * @return The uint32_t word which is suitable to print.
 */
uint32_t to_uint32_print(uint8_t byte_array[WORD_SIZE_IN_BYTES]);

/**
 * @brief Stores the contents of the register back into memory.
 * 
 * @param word The uint32_t word of the register.
 * @param byte_array The byte array.
 */
void to_uint8_array(uint32_t word, uint8_t byte_array[WORD_SIZE_IN_BYTES]);

#define EXTRACT_BITS(raw, from, size)                                          \
  (uint32_t)((raw >> (uint8_t)from) & ((1 << (uint8_t)size) - 1))

#define EXTRACT_BIT(raw, pos)                                                  \
  (uint32_t)((raw >> pos) & 1)


#endif
