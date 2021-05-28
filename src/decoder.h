#ifndef DECODER_H
#define DECODER_H

#include "emulate.h"

/**
 * @brief 
 * Takes an instruction containing raw uint32_t data, and decodes it to
 * a struct of instr_data.
 *
 * If the instruction is 0x0 (HALT signal), or if it cannot be decoded, it tags
 * the instruction_t with HALT.
 * @param *instr The input instruction.
 */
instruction_t *decode(instruction_t *instr);

#endif /* DECODER_H */
