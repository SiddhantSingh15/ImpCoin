#include <stdlib.h>
#include <stdbool.h>
#include "definitions.h"
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
