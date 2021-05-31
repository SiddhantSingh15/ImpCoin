#ifndef DATAPROC_OPERATIONS_H
#define DATAPROC_OPERATIONS_H

#include <stdint.h>

//returns result for 'and' and 'tst' operations.
int32_t and_tst(int32_t op1, int32_t op2, int *carry_out);

int32_t eor_teq(int32_t op1, int32_t op2, int *carry_out);

int32_t sub(int32_t op1, int32_t op2, int *carry_out);

int32_t rsb(int32_t op1, int32_t op2, int *carry_out);

int32_t add(int32_t op1, int32_t op2, int *carry_out);

int32_t cmp(int32_t op1, int32_t op2, int *carry_out);

int32_t orr(int32_t op1, int32_t op2, int *carry_out);

int32_t mov(int32_t op1, int32_t op2, int *carry_out);

uint32_t lsl(uint32_t to_shift, uint8_t shift_amt, int *carry_out);

uint32_t lsr(uint32_t to_shift, uint8_t shift_amt, int *carry_out);

uint32_t asr(uint32_t to_shift, uint8_t shift_amt, int *carry_out);

uint32_t ror(uint32_t to_rotate, uint8_t rotate_amt, int *carry_out);

typedef int32_t (*data_operation)(int32_t, int32_t, int *);
typedef uint32_t (*shift_operation)(uint32_t, uint8_t, int *);

#endif
