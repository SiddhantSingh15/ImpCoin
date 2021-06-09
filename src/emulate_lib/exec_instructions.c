#include "../global_helpers/definitions.h"
#include "../global_helpers/types.h"
#include "emulate_utils.h"
#include "exec_utils.h"
#include "dataproc_operations.h"
#include "gpio.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* The order in which operations are declared matters
 * as they correspond to the opcodes*/
const data_operation *get_operations() {
  const static data_operation operations[] = {
      and_tst, eor_teq, sub,     rsb, add,  NULL, NULL,
      NULL,    and_tst, eor_teq, cmp, NULL, orr,  mov};
  return operations;
}

void exec_dataproc(dataproc_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file)) {
    return;
  }

  int carry_out = 0;
  int32_t operand1 = state->register_file[instr.rn];
  // Changes the current Operand2 based on the value of the I flag.
  int32_t operand2 = barrel_shifter(instr.is_immediate, instr.op2,
                                    state->register_file, &carry_out);

  uint32_t result = 0;
  if (is_arithmetic(instr.opcode) || is_logic(instr.opcode)) {
    result = get_operations()[instr.opcode](operand1, operand2, &carry_out);
  } else {
    printf("Illegal data processing instruction!");
    return;
  }

  if (!discards_result(instr.opcode)) {
    state->register_file[instr.rd] = result;
  }

  // Sets the COND flags if the C flag is 1.
  if (instr.set_cond) {
    set_flag(state->register_file, EXTRACT_BIT(result, N_FLAG), N_FLAG);
    set_flag(state->register_file, carry_out, C_FLAG);
    set_flag(state->register_file, result == 0, Z_FLAG);
  }
}

void exec_branch(branch_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file)) {
    return;
  }

  state->register_file[PC] =
      (int32_t)state->register_file[PC] + signed_24_to_32(instr.offset << 2);
  flush_pipeline(state->pipeline);
}

void exec_sdt(sdt_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file)) {
    return;
  }

  assert(PC != instr.rd);
  assert(PC != instr.offset);
  if (instr.is_shift_R) {
    // PC cannot be equal to the shift register (R_m).
    assert(PC != EXTRACT_BITS(instr.offset, RM_POS, REG_SIZE));
  }

  int32_t interpreted_offset = instr.offset;
  if (instr.is_shift_R) {
    int carry_out = RESET;
    interpreted_offset = barrel_shifter(!instr.is_shift_R, instr.offset,
                                        state->register_file, &carry_out);
    // set_flag(state->register_file, carry_out, C_FLAG);
  }

  uint32_t mem_address = state->register_file[instr.rn];

  if (instr.is_preindexed) {
    mem_address +=
        (instr.up_bit ? interpreted_offset : twos_comp(interpreted_offset));
  }
  if (mem_address <= MEM_SIZE) {
    if (instr.load) {
      // Loads the memory to R[instr.rd], after converting it to word size
      state->register_file[instr.rd] =
          to_uint32_reg(&state->main_memory[mem_address]);
    } else {
      // Stores the value at instr.rd, after converting it to a byte array
      to_uint8_array(state->register_file[instr.rd],
                     &state->main_memory[mem_address]);
    }

  } else if ((mem_address >> GPIO_BITMASK) == GPIO_INDIC) {
    gpio_access(state, instr.rd, mem_address);
  } else {
    printf("Error: Out of bounds memory access at address 0x%08x\n",
           mem_address);
  }
  if (mem_address <= MEM_SIZE) {
    if (instr.load) {
      // Loads the memory to R[instr.rd], after converting it to word size
      state->register_file[instr.rd] =
          to_uint32_reg(&state->main_memory[mem_address]);
    } else {
      // Stores the value at instr.rd, after converting it to a byte array
      to_uint8_array(state->register_file[instr.rd],
                     &state->main_memory[mem_address]);
    }


  if (instr.is_preindexed == 0) {
    state->register_file[instr.rn] +=
        ((instr.up_bit == 1) ? interpreted_offset : -1 * interpreted_offset);
  }
}

void exec_mult(multiply_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file)) {
    return;
  }

  // Assumptions
  assert(instr.rm != PC && instr.rn != PC && instr.rd != PC && instr.rs != PC);
  assert(instr.rd != instr.rm);

  // Multiplication Procedure
  uint32_t result =
      state->register_file[instr.rm] * state->register_file[instr.rs];
  if (instr.accumulate) {
    // Multiple-Accumulate - Rd = Rm * Rs + Rn
    // Add the value in Rn to result (result = Rm * Rs at this point).
    result += state->register_file[instr.rn];
  }
  state->register_file[instr.rd] = result;

  if (instr.set_cond) {
    set_flag(state->register_file, EXTRACT_BIT(result, N_FLAG), N_FLAG);
    set_flag(state->register_file, result == 0, Z_FLAG);
  }
}

void execute(instruction_t *instr, arm11_state_t *state) {
  uint32_t raw;
  switch (instr->tag) {
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
        raw);
    exit(EXIT_FAILURE);
  }
}
