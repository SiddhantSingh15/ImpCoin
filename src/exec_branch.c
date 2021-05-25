#include "emulate.h"

void exec_branch (branch_t instr) {
  //offset, cond
  if (!cpsr_checker())
    then
      return;
  instr.offset = int32_t (instr.offset << 2);
  //add to pc
}

void exec_sdt (sdt_t instr) {
  if (!cpsr_checker())
    then
      return;
  if (instr.is_shift == 1)
}

