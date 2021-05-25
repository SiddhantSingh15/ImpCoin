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
  
  uint32_t zero_checker;

  // TODO: Implement rotate and barrel shifter
  if (instr.is_immediate) {
    instr.op2 = rotate(instr, state->register_file);
  } else {  
    instr.op2 = barrel_shifter(instr, state->register_file);
  }

  switch (instr.opcode) {

    case AND:
      state->register_file[instr.rd] 
        = state->register_file[instr.rn] & instr.op2;

    case EOR:
      state->register_file[instr.rd] 
        = state->register_file[instr.rn] ^ instr.op2;

      zero_checker = state->register_file[instr.rd];

    case SUB:
      if (overflow(state->register_file[instr.rn], twos_comp(instr.op2))) {
        set_flag(state->register_file, NOT_SET, C_FLAG);
      } else {
        set_flag(state->register_file, SET, C_FLAG);
      }

      state->register_file[instr.rd] 
        = state->register_file[instr.rn] - instr.op2;

      zero_checker = state->register_file[instr.rd];

    case RSB:
      if (overflow(twos_comp(instr.op2), state->register_file[instr.rn])) {
        set_flag(state->register_file, NOT_SET, C_FLAG);
      } else if (state->register_file[instr.rn] > instr.op2) {
        set_flag(state->register_file, NOT_SET, C_FLAG);
      } else {
        set_flag(state->register_file, SET, C_FLAG);
      }

      state->register_file[instr.rd]
        = instr.op2 - state->register_file[instr.rn];

      zero_checker = state->register_file[instr.rd];
    
    case ADD:
      if (overflow(instr.op2, state->register_file[instr.rn])) {
        set_flag(state->register_file, SET, C_FLAG);
      } else {
        set_flag(state->register_file, NOT_SET, C_FLAG);
      }

      state->register_file[instr.rd]
        = state->register_file[instr.rn] + instr.op2;

      zero_checker = state->register_file[instr.rd];

    case TST:
      zero_checker = state->register_file[instr.rn] & instr.op2;

    case TEQ:
      zero_checker = state->register_file[instr.rn] ^ instr.op2;

    case CMP:
      if (state->register_file[instr.rn] < instr.op2) {
        set_flag(state->register_file, NOT_SET, C_FLAG);
      } else {
        set_flag(state->register_file, SET, C_FLAG);
      }

      zero_checker = state->register_file[instr.rn] - instr.op2;

    case ORR:
      state->register_file[instr.rd]
        = state->register_file[instr.rn] | instr.op2;

      zero_checker =  state->register_file[instr.rd];
    
    case MOV:
      state->register_file[instr.rd] = instr.op2;
      
      zero_checker = state->register_file[instr.rd];
    
    default:
      printf("Illegal data processing instruction !");
  }

  if (instr.set_cond) {
    set_flag(state->register_file, EXTRACT_BIT(zero_checker, N_FLAG), N_FLAG);

    if (zero_checker == 0) {
      set_flag(state->register_file, SET, Z_FLAG);
    } else {
      set_flag(state->register_file, NOT_SET, Z_FLAG);
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
