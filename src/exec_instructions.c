#include "definitions.h"
#include "emulate.h"
#include "exec_utils.h"
#include <assert.h>
#include <stdint.h>

// TODO: take data proc steps when offset must be interpreted as shifted
// register
uint16_t as_shifted_register(uint16_t offset);

void exec_branch(branch_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file))
    return;
  state->register_file[PC] =
      state->register_file[PC] - 8 + (int32_t)(instr.offset << 2);
}

void exec_sdt(sdt_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file))
    return;

  uint8_t pc = state->register_file[PC];

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

  if (instr.load == SET)
    state->register_file[instr.rd] = state->main_memory[mem_address];
  else
    state->main_memory[mem_address] = state->register_file[instr.rd];

  if (instr.is_preindexed == 0) {
    state->register_file[instr.rn] +=
        ((instr.up_bit == 1) ? interpreted_offset : -1 * interpreted_offset);
  }
}
