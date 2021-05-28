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
 * @brief Executes the Single Data Transfer Instruction.
 *
 * @param instr The input instruction.
 * @param state The current state of the ARM11 system.
 */
void exec_sdt(sdt_t instr, arm11_state_t *state);

/**
 * @brief Executes the Multiply Instruction.
 *
 * @param instr The input instruction.
 * @param state The current state of the ARM11 system.
 */
void exec_mult(multiply_t instr, arm11_state_t *state);

/**
 * @brief Executes the Branch Instruction.
 *
 * @param instr The input instruction.
 * @param state The current state of the ARM11 system.
 */
void exec_branch(branch_t instr, arm11_state_t *state);

/**
 * @brief Takes a pointer to an instruction and updates the internal memory 
 * and registers to reflect the executed operation.
 *
 * @param instr The pointer to the instruction.
 * @param state The pointer to the state of the ARM11 system.
 */
void execute(instruction_t *instr, arm11_state_t *state);

#endif // EXEC_INSTRUCTIONS
