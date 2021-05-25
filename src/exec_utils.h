#ifndef EXEC_UTILS_H
#define EXEC_UTILS_H

#include "definitions.h"
#include <stdint.h>
#include <stdbool.h>

bool satisfies_cpsr(uint8_t cond, uint32_t regs[NUM_REGS]);


#endif /* EXEC_UTILS_H */
