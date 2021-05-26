#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "definitions.h"
#include "emulate.h"
#include "exec_utils.h"
#include "decoder.h"

#define EXTRACT_BIT(raw, pos)                                                  \
  (uint32_t)(((1 << (uint8_t)pos) & (uint8_t)raw) >> (uint8_t)pos)

bool satisfies_cpsr(uint8_t cond, uint32_t regs[NUM_REGS]) {
  uint32_t cpsr_register = regs[CPSR];
  bool n = EXTRACT_BIT(cpsr_register, N_FLAG);
  bool z = EXTRACT_BIT(cpsr_register, Z_FLAG);
  bool c = EXTRACT_BIT(cpsr_register, C_FLAG);
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

bool overflow(uint32_t x, uint32_t y) {
  return (x > (INT_MAX - y) || y > (INT_MAX - x));
}

uint32_t twos_comp(uint8_t x) {
  return ~x + 1;
}

void set_flag(uint32_t *reg_file, int set, int flag) {
  if (set) { 
    reg_file[CPSR] |= set << flag;
  } else {
    reg_file[CPSR] &= ~(set << flag);
  }
}

bool is_logic(int val) {
  return 
    (val == AND 
    || val == EOR 
    || val == TST 
    || val == TEQ 
    || val == ORR 
    || val == MOV);
}

bool is_arithmetic(int val) {
  return 
    (val == SUB 
    || val == RSB 
    || val == ADD 
    || val == CMP);
}

void rotate(dataproc_t *instr, uint32_t *reg_file) {
  uint8_t rotate_amt = 2 * EXTRACT_BITS(instr->op2, 8, 4);
  uint32_t imm_value = EXTRACT_BITS(instr->op2, 0, 8);

  uint32_t result 
    = (imm_value << rotate_amt) | (imm_value >> (INSTR_SIZE - rotate_amt));

  instr->op2 = result;

  /* Setting the C FLAG in CPSR if the S FLAG is 1. */
  if (instr->set_cond) {
    set_flag(&reg_file[CPSR], EXTRACT_BIT(result, 31), C_FLAG);
  }
}

void barrel_shifter(dataproc_t *instr, uint32_t *reg_file) {
  uint32_t rm_addr = EXTRACT_BITS(instr->op2, 0, 4);
  uint8_t shift = EXTRACT_BITS(instr->op2, 4, 8);
  uint8_t option_flag = EXTRACT_BIT(shift, 0);
  uint32_t rm_reg = reg_file[rm_addr];
  uint8_t shift_amt;
  uint32_t result;

  if (option_flag) {
    shift_amt = 
  } else {
    shift_amt = 
  }

  if (shift_amt) {
    switch(shift) {
      case LSL:
        rm_reg <<= shift_amt - 1;
        if (instr->set_cond) {
          set_flag(&reg_file[CPSR], EXTRACT_BIT(rm_reg, 31), C_FLAG);
        }
        result = rm_reg >> 1; 

      case LSR:
        rm_reg >>= shift_amt - 1;
        if (instr->set_cond) {
          set_flag(&reg_file[CPSR], EXTRACT_BIT(rm_reg, 31), C_FLAG);
        }
        result = rm_reg >> 1;

      case ASR:
        
      case ROR:
      
      default: 

    }
  }

  instr->op2 = result;
}