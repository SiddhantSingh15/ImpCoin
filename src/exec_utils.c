#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "definitions.h"
#include "emulate.h"
#include "exec_utils.h"

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

//TODO: finish this...
void rotate(dataproc_t instr, uint32_t *reg_file) {
  uint32_t rotate_amt = 2 * EXTRACT_BITS(instr.op2, 8, 4);

  if (is_logic(instr.opcode)) {

  }
}