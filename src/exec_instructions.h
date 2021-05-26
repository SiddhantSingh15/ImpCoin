#ifndef EXEC_INSTRUCTIONS_H
#define EXEC_INSTRUCTIONS_H

#include "exec_utils.h"

// // Execute dataproc
// void exec_dataproc(dataproc_t instr, arm11_state_t *state);

// // Execute SDT
// void exec_sdt(sdt_t instr, arm11_state_t *state);

// // Execute Mult
// void exec_mult(multiply_t instr, arm11_state_t *state);

// // Execute Branch
// void exec_branch(branch_t instr, arm11_state_t *state);

// Execute function
void execute(instruction_t *instr, arm11_state_t *state);

#endif // EXEC_INSTRUCTIONS