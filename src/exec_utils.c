#include <stdlib.h>
#include <stdbool.h>
#include "definitions.h"

bool cpsr_checker(uint8_t cond, uint32_t regs[NUM_REGS]) {
  uint32_t cpsr_register = {0};
  memcpy(cpsr_register, regs[CPSR], REG_SIZE);
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
