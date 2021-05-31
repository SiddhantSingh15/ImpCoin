#ifndef EXEC_UTILS_H
#define EXEC_UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../global_helpers/definitions.h"

/**
 * @brief Checks if the COND bits passed in satisfy the pre-defined constraints
 * set in the spec with regards to the CPSR bits.
 *
 * @param cond Input COND bits.
 * @param regs[NUM_REGS] Passes in current register file of the ARM11 System.
 *
 * @return True if the constraints are satisfied.
 *
 */
bool satisfies_cpsr(uint8_t cond, uint32_t regs[NUM_REGS]);

/**
 * @brief Checks if the addition of two values causes an overflow or underflow.
 *
 * @param x First input.
 * @param y Second input.
 *
 * @return true if there is an overflow or underflow.
 */
bool overflow(int32_t x, int32_t y);

/**
 * @brief Turns input into 2s complement.
 *
 * @param x Input
 *
 * @return 2s complement output.
 */
int32_t twos_comp(int32_t x);

/**
 * @brief Sign-extends a signed number to 32 bits
 *
 * @param num Input
 *
 * @return 32 bit extended version of num
 */
int32_t signed_24_to_32(uint32_t num);

/**
 * @brief Sets corresponding flag in the CPSR register.
 *
 * @param reg_file The current register state of the ARM11 system.
 * @param set Either 0 or 1
 * @param flag The flag the user wants to change.
 */
void set_flag(uint32_t *reg_file, bool set, int flag);

/**
 * @brief Checks if the instruction type is logical.
 *
 * @param val The instruction type.
 *
 * @return Returns true if val is of logical type.
 */
bool is_logic(int val);

/**
 * @brief Checks if the instruction type is arithmetic.
 *
 * @param val The instruction type.
 *
 * @return Returns true if val is of arithmetic type.
 */
bool is_arithmetic(int val);

/**
 * @brief Checks if the operation type discards result i.e. does not save result
 * to register.
 *
 * @param val The opcode of instruction.
 *
 * @return Returns true if the operation discards result.
 */
bool discards_result(int val);

/**
 * @brief Applies shifts to the input register based on the shift type.
 *
 * @param is_immediate I flag
 * @param offset Input register.
 * @param register_file The current registers of the ARM11 system.
 */
uint32_t barrel_shifter(bool is_immediate, uint16_t offset,
                        uint32_t *register_file, int *carry_out);

#endif /* EXEC_UTILS_H */
