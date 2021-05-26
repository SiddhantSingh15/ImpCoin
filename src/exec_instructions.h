#include "definitions.h"
#include "emulate.h"
#include "emulate_utils.h"
#include "exec_utils.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>


// Execute dataproc
void exec_dataproc(dataproc_t instr, arm11_state_t *state);

// Execute SDT
void exec_sdt(sdt_t instr, arm11_state_t *state);

// Execute Mult
void exec_mult(multiply_t instr, arm11_state_t *state);

// Execute Branch
void exec_branch(branch_t instr, arm11_state_t *state);

// Execute function
void execute(instruction_t *instr, arm11_state_t *state);
