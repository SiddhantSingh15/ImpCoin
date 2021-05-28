#include <stdint.h>
#include "../global_helpers/definitions.h"
#include "exec_utils.h"

int32_t and_tst(int32_t op1, int32_t op2, int *carry_out){
  return op1 & op2;
}

int32_t eor_teq(int32_t op1, int32_t op2, int *carry_out){
  return op1 ^ op2;
}

int32_t sub(int32_t op1, int32_t op2, int *carry_out){
  *carry_out = !overflow(op1, twos_comp(op2));
  return op1 - op2;
}

int32_t rsb(int32_t op1, int32_t op2, int *carry_out){
  *carry_out = !overflow(op2, twos_comp(op1));
  return op2 - op1;
}

int32_t add(int32_t op1, int32_t op2, int *carry_out){
  *carry_out = overflow(op1, op2);
  return op1 + op2;
}

int32_t cmp(int32_t op1, int32_t op2, int *carry_out){
  *carry_out = !(op1 < op2);
  return op1 - op2;
}

int32_t orr(int32_t op1, int32_t op2, int *carry_out){
  return op1 | op2;
}

int32_t mov(int32_t op1, int32_t op2, int *carry_out){
  return op2;
}
