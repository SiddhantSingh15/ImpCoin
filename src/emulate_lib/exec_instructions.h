#ifndef EXEC_INSTRUCTIONS_H
#define EXEC_INSTRUCTIONS_H

#include "exec_utils.h"

/**
 * @brief Executes the Data Processing Instruction.
 *
 * @param instr The input instruction.
 * @param state The current state of the ARM11 system.
 */
void exec_dataproc(dataproc_t instr, arm11_state_t *state);

/**
 * @brief
 *
 * @param
 * @param
 */
void exec_sdt(sdt_t instr, arm11_state_t *state);

/**
 * @brief
 *
 * @param
 * @param
 */
void exec_mult(multiply_t instr, arm11_state_t *state);

/**
 * @brief
 *
 * @param
 * @param
 */
void exec_branch(branch_t instr, arm11_state_t *state);

/**
 * @brief
 *
 * @param
 * @param
 */
void execute(instruction_t *instr, arm11_state_t *state);

#endif // EXEC_INSTRUCTIONS
