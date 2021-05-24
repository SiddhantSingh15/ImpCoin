#include "emulate.h"
#include <assert.h>
#include <stdint.h>

#define EXTRACT_BITS(raw, from, size)                                          \
  (uint32_t)(                                                                  \
      ((((1 << (uint8_t)size) - 1) << (uint8_t)from) & (uint32_t)raw) >>       \
      (uint8_t)from)

#define EXTRACT_BIT(raw, pos)                                                  \
  (uint32_t)(((1 << (uint8_t)pos) & (uint8_t)raw) >> (uint8_t)pos)

// TODO: Macros or functions?

/*
uint32_t extract_bits(uint32_t raw, uint8_t from, uint8_t size) {
  return ((((1 << size) - 1) << from) & raw) >> from;
}

uint8_t extract_bit(uint32_t raw, uint8_t position) {
  return ((1 << position) & raw) >> position;
}
*/

instruction_t *decode_dataproc(instruction_t *instr) {
  // the instruction to be decoded must be raw
  assert(instr->tag == RAW);

  uint32_t raw_data = instr->data.raw_data;

  instr->data.dataproc.cond = EXTRACT_BITS(raw_data, 28, 4);
  instr->data.dataproc.is_immediate = EXTRACT_BIT(raw_data, 25);
  instr->data.dataproc.opcode = EXTRACT_BITS(raw_data, 21, 4);
  instr->data.dataproc.set_cond = EXTRACT_BIT(raw_data, 21);
  instr->data.dataproc.rn = EXTRACT_BITS(raw_data, 16, 4);
  instr->data.dataproc.rd = EXTRACT_BITS(raw_data, 12, 4);
  instr->data.dataproc.op2 = EXTRACT_BITS(raw_data, 0, 12);

  instr->tag = DATAPROC;
  return instr;
}

instruction_t *decode_multiply(instruction_t *instr) {
  // the instruction to be decoded must be raw
  assert(instr->tag == RAW);

  uint32_t raw_data = instr->data.raw_data;

  instr->data.multiply.cond = EXTRACT_BITS(raw_data, 28, 4);
  instr->data.multiply.accumulate = EXTRACT_BIT(raw_data, 21);
  instr->data.multiply.set_cond = EXTRACT_BIT(raw_data, 20);
  instr->data.multiply.rd = EXTRACT_BITS(raw_data, 16, 4);
  instr->data.multiply.rn = EXTRACT_BITS(raw_data, 12, 4);
  instr->data.multiply.rs = EXTRACT_BITS(raw_data, 0, 12);

  instr->tag = MULTIPLY;
  return instr;
}

instruction_t *decode_sdt(instruction_t *instr) {
  // the instruction to be decoded must be raw
  assert(instr->tag == RAW);

  uint32_t raw_data = instr->data.raw_data;

  instr->data.sdt.cond = EXTRACT_BITS(raw_data, 28, 4);
  instr->data.sdt.is_offset = EXTRACT_BIT(raw_data, 25);
  instr->data.sdt.is_preindexed = EXTRACT_BIT(raw_data, 24);
  instr->data.sdt.load = EXTRACT_BIT(raw_data, 20);
  instr->data.sdt.rn = EXTRACT_BITS(raw_data, 16, 4);
  instr->data.sdt.rd = EXTRACT_BITS(raw_data, 12, 4);
  instr->data.sdt.offset = EXTRACT_BITS(raw_data, 0, 12);

  instr->tag = SDT;
  return instr;
}

instruction_t *decode_branch(instruction_t *instr) {
  // the instruction to be decoded must be raw
  assert(instr->tag == RAW);

  uint32_t raw_data = instr->data.raw_data;

  instr->data.branch.cond = EXTRACT_BITS(raw_data, 28, 4);
  instr->data.branch.offset = EXTRACT_BITS(raw_data, 0, 24);

  instr->tag = BRANCH;
  return instr;
}

instruction_t *decode(instruction_t *raw) {
  // the instruction to be decoded must be raw
  assert(raw->tag == RAW);
}
