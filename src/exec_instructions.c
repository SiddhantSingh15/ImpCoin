#include "definitions.h"
#include "emulate.h"
#include "emulate_utils.h"
#include "exec_utils.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void exec_dataproc(dataproc_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file))
    return;

  uint32_t result = 0;
  int carry_out = 0;

  int32_t operand1 = state->register_file[instr.rn];
  // Changes the current Operand2 based on tehe value of the I flag.
  int32_t operand2 = barrel_shifter(instr.is_immediate, instr.op2,
                                    state->register_file, &carry_out);

  // Switch condition to perform the operations specified by the instruction
  // OpCode.
  switch (instr.opcode) {

  case AND:
    state->register_file[instr.rd] = operand1 & operand2;

    result = state->register_file[instr.rd];
    break;

  case EOR:
    state->register_file[instr.rd] = operand1 ^ operand2;

    result = state->register_file[instr.rd];
    break;

  case SUB:
    carry_out = !overflow(operand1, twos_comp(operand2));

    state->register_file[instr.rd] = operand1 - operand2;

    result = state->register_file[instr.rd];
    break;

  case RSB:
    carry_out = !overflow(operand2, twos_comp(operand1));

    state->register_file[instr.rd] = operand2 - operand1;

    result = state->register_file[instr.rd];
    break;

  case ADD:
    carry_out = overflow(operand1, operand2);

    state->register_file[instr.rd] = operand1 + operand2;

    result = state->register_file[instr.rd];
    break;

  case TST:
    result = operand1 & operand2;
    break;

  case TEQ:
    result = operand1 ^ operand2;
    break;

  case CMP:
    carry_out = !(operand1 < operand2);

    result = operand1 - operand2;
    break;

  case ORR:
    state->register_file[instr.rd] = operand1 | operand2;

    result = state->register_file[instr.rd];
    break;

  case MOV:
    state->register_file[instr.rd] = operand2;

    result = state->register_file[instr.rd];
    break;

  default:
    printf("Illegal data processing instruction !");
  }

  // Sets the COND flags if the C flag is 1.
  if (instr.set_cond) {
    set_flag(state->register_file, EXTRACT_BIT(result, N_FLAG), N_FLAG);
    set_flag(state->register_file, carry_out, C_FLAG);
    set_flag(state->register_file, result == 0, Z_FLAG);
  }
}

void exec_branch(branch_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file))
    return;

  state->register_file[PC] =
      state->register_file[PC] + signed_24_to_32(instr.offset);
  flush_pipeline(state->pipeline);
}

void exec_sdt(sdt_t instr, arm11_state_t *state) {
  if (!satisfies_cpsr(instr.cond, state->register_file))
    return;

  assert(PC != instr.rd);
  assert(PC != instr.offset);
  if (instr.is_shift_R) {
    // PC cannot be equal to the shift register (R_m).
    assert(PC != EXTRACT_BITS(instr.offset, RM_POS, REG_SIZE));
  }

  uint32_t interpreted_offset = instr.offset;
  if (instr.is_shift_R == 1) {
    int carry_out = 0;
    interpreted_offset = barrel_shifter(!instr.is_shift_R, instr.offset,
                                        state->register_file, &carry_out);
    // set_flag(state->register_file, carry_out, C_FLAG);
  }

  uint16_t mem_address = state->register_file[instr.rn];

  if (instr.is_preindexed == 1) {
    mem_address +=
        ((instr.up_bit == 1) ? interpreted_offset : -1 * interpreted_offset);
  }

  if (instr.load == SET) {
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
