#ifndef EXEC_UTILS_H
#define EXEC_UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

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
 * @brief Checks if the input values cause an overflow.
 *
 * @param x First input.
 * @param y Second input.
 *
 * @return true if there is an overflow.
 */
bool overflow(uint32_t x, uint32_t y);

/**
 * @brief Turns input into 2s complement.
 *
 * @param x Input
 *
 * @return 2s complement output.
 */
uint32_t twos_comp(uint8_t x);

/**
 * @brief Sets corresponding flag in the CPSR register.
 *
 * @param reg_file The current register state of the ARM11 system.
 * @param set Either 0 or 1
 * @param flag The flag the user wants to change.
 */
void set_flag(uint32_t *reg_file, int set, int flag);

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
 * @brief Rotates the register to the right specified by input params.
 *
 * @param to_rotate The input register to rotate.
 * @param rotate_amt The amount the register should be rotated by.
 *
 * @return The rotated register.
 */
uint32_t rotate_right(uint32_t to_rotate, uint8_t rotate_amt);

/**
 * @brief Applies shifts to the input register based on the shift type.
 *
 * @param is_immediate I flag
 * @param offset Input register.
 * @param register_file The current registers of the ARM11 system.
 */
uint32_t barrel_shifter(bool is_immediate, bool set_cond, uint16_t offset,
                        uint32_t *register_file);

#endif /* EXEC_UTILS_H */
