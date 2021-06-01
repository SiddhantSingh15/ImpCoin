#include "../global_helpers/definitions.h"
#include "dataproc_operations.h"
#include "exec_utils.h"
#include "emulate_utils.h"
#include <stdint.h>
#include <stdlib.h>

/*------------------------------------------------------------
 * Arithmetic and Logical Data Processing Functions.
 * */

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

/*------------------------------------------------------------
 * Shift and Rotate Functions
 * */

uint32_t lsl(uint32_t to_shift, uint8_t shift_amt, int *carry_out) {
  *carry_out = EXTRACT_BIT(to_shift, (INSTR_SIZE - shift_amt + 1));
  return to_shift << shift_amt;
}

uint32_t lsr(uint32_t to_shift, uint8_t shift_amt, int *carry_out) {
  *carry_out = EXTRACT_BIT(to_shift, (shift_amt - 1));
  return to_shift >> shift_amt;
}

uint32_t asr(uint32_t to_shift, uint8_t shift_amt, int *carry_out) {
  *carry_out = EXTRACT_BIT(to_shift, (shift_amt - 1));
  uint32_t result = to_shift >> shift_amt;
  uint8_t mask = (int32_t)result < 0 ? 1 << (shift_amt - 1) : 0;
  return result |= (mask << (INSTR_SIZE - shift_amt));
}

uint32_t ror(uint32_t to_rotate, uint8_t rotate_amt, int *carry_out) {
  *carry_out = EXTRACT_BIT(to_rotate, (rotate_amt - 1));
  return (to_rotate >> rotate_amt) | (to_rotate << (INSTR_SIZE - rotate_amt));
}
