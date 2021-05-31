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

typedef int32_t (*data_operation)(int32_t, int32_t, int *);

#endif
