#include "definitions.h"
#include "emulate.h"
#include "emulate_utils.h"
#include "exec_utils.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Executes the Data Processing Instruction.
 * 
 * @param instr The input instruction.
 * @param state The current state of the ARM11 system.
 */

void exec_dataproc(dataproc_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file))
    return;
  
  uint32_t zero_checker;

  instr.op2 
    = barrel_shifter(instr.is_immediate, instr.op2, state->register_file);
  

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
/**
 * @brief
 * 
 * @param
 * @param
 */

void exec_branch(branch_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file))
    return;

  state->register_file[PC] =
      state->register_file[PC] - 8 + (int32_t)(instr.offset << 2);
}

/**
 * @brief
 * 
 * @param
 * @param
 */

void exec_sdt(sdt_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file))
    return;

  if (instr.rn == PC)
    state->register_file[instr.rn] += 8;

  assert(PC != instr.rd);
  assert(PC != instr.offset);

  uint32_t interpreted_offset = instr.offset;
  if (instr.is_shift_R == 1) {
    interpreted_offset 
      = barrel_shifter(!instr.is_shift_R, instr.offset, state->register_file);
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

/**
 * @brief
 * 
 * @param
 * @param
 */
void exec_mult(multiply_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file))
    return;

  // Assumptions
  assert (instr.rm != PC && instr.rn != PC && instr.rd != PC && instr.rs != PC);
  assert (instr.rd != instr.rm);

  // Multiplication Procedure
  uint32_t result = state->register_file[instr.rm] * state->register_file[instr.rs];
  if (instr.accumulate) {
    // Multiple-Accumulate - Rd = Rm * Rs + Rn
    // Add the value in Rn to result (result = Rm * Rs at this point).
    result += state->register_file[instr.rn];
  }
  state->register_file[instr.rd] = result;

  if (instr.set_cond) {
    if (instr.set_cond) {
    set_flag(state->register_file, EXTRACT_BIT(result, N_FLAG), N_FLAG);

    if (result == 0) {
      set_flag(state->register_file, SET, Z_FLAG);
    } else {
      set_flag(state->register_file, NOT_SET, Z_FLAG);
    }

  }
  }
}

/**
 * @brief
 * 
 * @param
 * @param
 */
void execute(instruction_t *instr, arm11_state_t *state) {
  uint32_t raw;
  switch(instr->tag) {
    case DATAPROC:
      exec_dataproc(instr->data.dataproc, state);
      break;
    case MULTIPLY:
      exec_mult(instr->data.multiply, state);
      break;
    case SDT:
      exec_sdt(instr->data.sdt, state);
      break;
    case BRANCH:
      exec_branch(instr->data.branch, state);
      break;
    case RAW:
    default:
      // Unexpected Result - Executer encountered a RAW or a HALT instruction
      raw = instr->data.raw_data;
      fprintf(
        stderr,
        "0x%08x is a RAW or a HALT instruction in execute. Halting emulator.",
        raw
      );
      exit(EXIT_FAILURE);
  }
}
