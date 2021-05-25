#include "definitions.h"
#include "emulate.h"
#include "emulate_utils.h"
#include "exec_utils.h"
#include <assert.h>
#include <stdint.h>

// TODO: take data proc steps when offset must be interpreted as shifted
// register
uint16_t as_shifted_register(uint16_t offset);

void exec_dataproc(dataproc_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file))
    return;
  
  uint32_t result;

  if (instr.is_immediate) {

  } else {

  }

  switch (instr.opcode) {
    case AND:
      
  }

  if (instr.set_cond) {
    if (result == 0) {

    } else {

    }
  }

}

void exec_branch(branch_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file))
    return;

  state->register_file[PC] =
      state->register_file[PC] - 8 + (int32_t)(instr.offset << 2);
}

void exec_sdt(sdt_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file))
    return;

  if (instr.rn == PC)
    state->register_file[instr.rn] += 8;

  assert(PC != instr.rd);
  assert(PC != instr.offset);

  uint32_t interpreted_offset = instr.offset;
  if (instr.is_shift_R == 1) {
    interpreted_offset = as_shifted_register(instr.offset);
  }

  uint16_t mem_address = state->register_file[instr.rn];

  if (instr.is_preindexed == 1) {
    mem_address +=
        ((instr.up_bit == 1) ? interpreted_offset : -1 * interpreted_offset);
  }

  if (instr.load == SET) {
    // Loads the memory to R[instr.rd], after converting it to word size
    state->register_file[instr.rd] =
        to_uint32(&state->main_memory[mem_address]);
  } else {
    // Stores the value at instr.rd, after converting it to a byte array
    to_uint8_array(state->register_file[instr.rd],
                   &state->main_memory[mem_address]);
  }

  if (instr.is_preindexed == 0) {
    // TODO: check the R_m != PC condition, add this after doing dataproc
    state->register_file[instr.rn] +=
        ((instr.up_bit == 1) ? interpreted_offset : -1 * interpreted_offset);
  }
}
