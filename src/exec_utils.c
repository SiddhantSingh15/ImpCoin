#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "definitions.h"
#include "emulate.h"
#include "emulate_utils.h"
#include "exec_utils.h"
#include "decoder.h"

/**
 * @brief Checks if the COND bits passed in satisfy the pre-defined constraints
 * set in the spec with regards to the CPSR bits.
 * 
 * @param cond Input COND bits.
 * @param regs[NUM_REGS] Passes in current register file of the ARM11 System.
 * 
 * @return True if the constraints are satisfied.
 * 
 */

bool satisfies_cpsr(uint8_t cond, uint32_t regs[NUM_REGS]) {
  uint32_t cpsr_register = regs[CPSR];
  bool n = EXTRACT_BIT(cpsr_register, N_FLAG);
  bool z = EXTRACT_BIT(cpsr_register, Z_FLAG);
  bool v = EXTRACT_BIT(cpsr_register, V_FLAG);

  switch (cond) {
    case EQ :
      return z;
    case NE :
      return !z;
    case GE :
      return n == v;
    case LT :
      return n != v;
    case GT :
      return !z && (n == v);
    case LE :
      return z | (n != v);
    case AL :
      return true;
    default:
      return false;
  }
}

/**
 * @brief Checks if the input values cause an overflow.
 * 
 * @param x First input.
 * @param y Second input.
 * 
 * @return true if there is an overflow.
 */

bool overflow(uint32_t x, uint32_t y) {
  return (x > (INT_MAX - y) || y > (INT_MAX - x));
}

/**
 * @brief Turns input into 2s complement.
 * 
 * @param x Input
 * 
 * @return 2s complement output. 
 */

uint32_t twos_comp(uint8_t x) {
  return ~x + 1;
}

/**
 * @brief Sets corresponding flag in the CPSR register.
 * 
 * @param reg_file The current register state of the ARM11 system.
 * @param set Either 0 or 1
 * @param flag The flag the user wants to change.
 */

void set_flag(uint32_t *reg_file, int set, int flag) {
  if (set) {
    reg_file[CPSR] |= set << flag;
  } else {
    reg_file[CPSR] &= ~(set << flag);
  }
}

/** 
 * @brief Checks if the instruction type is logical.
 * 
 * @param val The instruction type.
 * 
 * @return Returns true if val is of logical type.
 */

bool is_logic(int val) {
  return
    (val == AND
    || val == EOR
    || val == TST
    || val == TEQ
    || val == ORR
    || val == MOV);
}

/** 
 * @brief Checks if the instruction type is arithmetic.
 * 
 * @param val The instruction type.
 * 
 * @return Returns true if val is of arithmetic type.
 */

bool is_arith(int val) {
  return
    (val == SUB
    || val == RSB
    || val == ADD
    || val == CMP);
}

/**
 * @brief Rotates the register to the right specified by input params.
 * 
 * @param to_rotate The input register to rotate.
 * @param rotate_amt The amount the register should be rotated by.
 * 
 * @return The rotated register.
 */

uint32_t rotate_right(uint32_t to_rotate, uint8_t rotate_amt) {
  return (to_rotate << rotate_amt) | (to_rotate >> (INSTR_SIZE - rotate_amt));
}

/**
 * @brief Applies shifts to the input register based on the shift type.
 * 
 * @param is_immediate I flag
 * @param offset Input register.
 * @param register_file The current registers of the ARM11 system.
 */

uint32_t barrel_shifter(bool is_immediate, uint16_t offset,
                          uint32_t *register_file) {

  uint32_t to_shift;
  uint32_t result;
  uint8_t shift_type;
  uint8_t shift_amt;

  if (is_immediate) {
    to_shift = EXTRACT_BITS(offset, 0, 8);
    shift_amt = 2 * EXTRACT_BITS(offset, 8, 4);
    result = rotate_right(to_shift, shift_amt);
  } else {

    uint32_t to_shift = register_file[EXTRACT_BITS(offset, 0, 4)];
    uint8_t shift = EXTRACT_BITS(offset, 4, 8);

    if ((shift & 0x1) == 0) {
      shift_amt = EXTRACT_BITS(shift, 3, 5);
    } else {
      // Shift amount is the bottom byte of the register specified
      shift_amt = register_file[EXTRACT_BITS(shift, 4, 4)] & 0xf;
    }

    shift_type = EXTRACT_BITS(shift, 1, 2);

    switch (shift_type) {
    case LSL:
      result = to_shift << shift_amt;
      break;
    case ASR:
      result = to_shift >> shift_amt;
      bool top_bit = EXTRACT_BIT(to_shift, (INSTR_SIZE - 1));
      uint8_t mask =  top_bit == 1 ? top_bit << (shift_amt - 1) : 0;
      result |= (mask << (INSTR_SIZE - shift_amt));
      break;
    case LSR:
      result = to_shift >> shift_amt;
      break;
    case ROR:
      result = rotate_right(to_shift, shift_amt);
      break;
    }
  }

  // find carry
  bool carry;
  if (shift_type == LSL) {
    carry = EXTRACT_BIT(to_shift, (INSTR_SIZE - shift_amt + 1));
  } else {
    carry = EXTRACT_BIT(to_shift, (shift_amt - 1));
  }

  // save carry
  set_flag(register_file, carry, C_FLAG);

  return result;
}
