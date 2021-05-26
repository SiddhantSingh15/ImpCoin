#ifndef EXEC_UTILS_H
#define EXEC_UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

bool satisfies_cpsr(uint8_t cond, uint32_t regs[NUM_REGS]);
bool overflow(uint32_t x, uint32_t y);
uint32_t twos_comp(uint8_t x);
void set_flag(uint32_t *reg_file, int set, int flag);
bool is_logic(int val);
bool is_arithmetic(int val);
uint32_t rotate_right(uint32_t to_rotate, uint8_t rotate_amt);
uint32_t barrel_shifter(bool is_immediate, uint16_t offset,
                          uint32_t *register_file);

#endif /* EXEC_UTILS_H */
