#include "../global_helpers/definitions.h"
#include "exec_utils.h"
#include <stdint.h>
#include <stdlib.h>

int32_t and_tst(int32_t op1, int32_t op2, int *carry_out) { return op1 & op2; }

int32_t eor_teq(int32_t op1, int32_t op2, int *carry_out) { return op1 ^ op2; }

int32_t sub(int32_t op1, int32_t op2, int *carry_out) {
  *carry_out = !overflow(op1, twos_comp(op2));
  return op1 - op2;
}

int32_t rsb(int32_t op1, int32_t op2, int *carry_out) {
  *carry_out = !overflow(op2, twos_comp(op1));
  return op2 - op1;
}

int32_t add(int32_t op1, int32_t op2, int *carry_out) {
  *carry_out = overflow(op1, op2);
  return op1 + op2;
}

int32_t cmp(int32_t op1, int32_t op2, int *carry_out) {
  *carry_out = !(op1 < op2);
  return op1 - op2;
}

int32_t orr(int32_t op1, int32_t op2, int *carry_out) { return op1 | op2; }

int32_t mov(int32_t op1, int32_t op2, int *carry_out) { return op2; }

typedef int32_t (*data_operation)(int32_t, int32_t, int *);

/* The order in which operations are declared matters
 * as they correspond to the opcodes*/
/* const data_operation *get_operations() {
  const static data_operation operations[] = {
      and_tst, eor_teq, sub,     rsb, add,  NULL, NULL,
      NULL,    and_tst, eor_teq, cmp, NULL, orr,  mov};
  return operations;
} */
