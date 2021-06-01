#ifndef DATAPROC_OPERATIONS_H
#define DATAPROC_OPERATIONS_H

#include <stdint.h>


// ---------- Arithmetic and Logical Data Operations -----------

/**
 * @brief The type of the arithmetic and logical data operation functions used
 * in the execution of data processing instructions.
 */
typedef int32_t (*data_operation)(int32_t, int32_t, int *);

/**
 * @brief Performs a logical `and` of op1 and op2. Carry out is not affected.
 * Since the `tst` instruction performs the same operation, without saving,
 * this function can be reused.
 *
 * @param op1 The first operand.
 * @param op2 The second operand.
 * @param carry_out A pointer to the carry out to be updated.
 *
 * @return The result.
 */
int32_t and_tst(int32_t op1, int32_t op2, int *carry_out);

/**
 * @brief Performs a logical `eor` of op1 and op2. Carry out is not affected.
 * Since the `teq` instruction performs the same operation, without saving,
 * this function can be reused.
 *
 * @param op1 The first operand.
 * @param op2 The second operand.
 * @param carry_out A pointer to the carry out to be updated.
 *
 * @return The result.
 */
int32_t eor_teq(int32_t op1, int32_t op2, int *carry_out);

/**
 * @brief Subtracts op2 from op1 `(op1 - op2)`. Carry out is overwritten
 * accordingly.
 *
 * @param op1 The first operand.
 * @param op2 The second operand.
 * @param carry_out A pointer to the carry out to be updated.
 *
 * @return The result.
 */
int32_t sub(int32_t op1, int32_t op2, int *carry_out);

/**
 * @brief Subtracts op1 from op2 `(op2 - op1)`. Carry out is
 * overwritten accordingly.
 *
 * @param op1 The first operand.
 * @param op2 The second operand.
 * @param carry_out A pointer to the carry out to be updated.
 *
 * @return The result.
 */
int32_t rsb(int32_t op1, int32_t op2, int *carry_out);

/**
 * @brief Adds op1 and op2 `(op1 + op2)`. Carry out is overwritten
 * accordingly.
 *
 * @param op1 The first operand.
 * @param op2 The second operand.
 * @param carry_out A pointer to the carry out to be updated.
 *
 * @return The result.
 */
int32_t add(int32_t op1, int32_t op2, int *carry_out);

/**
 * @brief Subtracts op2 from op1 `(op2 - op1)`. Carry out will be 0 if op1 <
 * op2.
 *
 * @param op1 The first operand.
 * @param op2 The second operand.
 * @param carry_out A pointer to the carry out to be updated.
 *
 * @return The result.
 */
int32_t cmp(int32_t op1, int32_t op2, int *carry_out);

/**
 * @brief Performs a logical `or` of op1 and op2. Carry out is not affected.
 *
 * @param op1 The first operand.
 * @param op2 The second operand.
 * @param carry_out A pointer to the carry out to be updated.
 *
 * @return The result.
 */
int32_t orr(int32_t op1, int32_t op2, int *carry_out);

/**
 * @brief Returns the second operand. Carry out is not affected.
 *
 * @param op1 The first operand.
 * @param op2 The second operand.
 * @param carry_out A pointer to the carry out to be updated.
 *
 * @return The result.
 */
int32_t mov(int32_t op1, int32_t op2, int *carry_out);


// ---------- Arithmetic and Logical Shift Operations ----------

/**
 * @brief The type of the arithmetic and logical shift operation functions used
 * in the barrel shifter.
 */
typedef uint32_t (*shift_operation)(uint32_t, uint8_t, int *);

/**
 * @brief Performs a logicakl left shift. Carry out is overwritten accordingly.
 *
 *
 * @param to_shift The value to shift.
 * @param shift_amt The number of positions to shift by.
 * @param carry_out A pointer to the carry out to be updated.
 *
 * @return The shifted value.
 */
uint32_t lsl(uint32_t to_shift, uint8_t shift_amt, int *carry_out);

/**
 * @brief Performs a logical right shift. Carry out is overwritten accordingly.
 *
 * @param to_shift The value to shift.
 * @param shift_amt The number of positions to shift by.
 * @param carry_out A pointer to the carry out to be updated.
 *
 * @return The shifted value.
 */
uint32_t lsr(uint32_t to_shift, uint8_t shift_amt, int *carry_out);

/**
 * @brief Performs an arithmetic right shift. Carry out is overwritten
 * accordingly.
 *
 * @param to_shift The value to shift.
 * @param shift_amt The number of positions to shift by.
 * @param carry_out A pointer to the carry out to be updated.
 *
 * @return The shifted value.
 */
uint32_t asr(uint32_t to_shift, uint8_t shift_amt, int *carry_out);

/**
 * @brief Performs a right rotation. Carry out is overwritten accordingly.
 *
 * @param to_rotate The value to shift.
 * @param rotate_amt The number of positions to shift by.
 * @param carry_out A pointer to the carry out to be updated.
 *
 * @return The shifted value.
 */
uint32_t ror(uint32_t to_rotate, uint8_t rotate_amt, int *carry_out);

#endif /* DATAPROC_OPERATIONS_H */
