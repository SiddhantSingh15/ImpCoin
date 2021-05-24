#include "emulate.h"
#include <assert.h>
#include <stdint.h>

// TODO: Use macros instead of functions for extracting bits?

uint32_t extract_bits(uint32_t raw, uint8_t from, uint8_t to) {
  return ((((1 << to) - 1) << from) & raw) >> from;
}

uint8_t extract_bit(uint32_t raw, uint8_t position) {
  return ((1 << position) & raw) >> position;
}

instruction_t *decode_dataproc(instruction_t *instr) {
  // the instruction to be decoded must be raw
  assert(instr->tag == RAW);

  uint32_t raw_data = instr->data.raw_data;

  instr->data.dataproc.cond = extract_bits(raw_data, 28, 32);
  instr->data.dataproc.is_immediate = extract_bit(raw_data, 25);
  instr->data.dataproc.opcode = extract_bits(raw_data, 21, 25);
  instr->data.dataproc.set_cond = extract_bit(raw_data, 21);
  instr->data.dataproc.rn = extract_bits(raw_data, 16, 20);
  instr->data.dataproc.rd = extract_bits(raw_data, 12, 16);
  instr->data.dataproc.op2 = extract_bits(raw_data, 0, 12);

  instr->tag = DATAPROC;
  return instr;
}

instruction_t *decode_multiply(instruction_t *instr) {
  // the instruction to be decoded must be raw
  assert(instr->tag == RAW);

  uint32_t raw_data = instr->data.raw_data;

  instr->data.multiply.cond = extract_bits(raw_data, 28, 32);
  instr->data.multiply.accumulate = extract_bit(raw_data, 21);
  instr->data.multiply.set_cond = extract_bit(raw_data, 20);
  instr->data.multiply.rd = extract_bits(raw_data, 16, 20);
  instr->data.multiply.rn = extract_bits(raw_data, 12, 16);
  instr->data.multiply.rs = extract_bits(raw_data, 0, 12);

  instr->tag = MULTIPLY;
  return instr;
}

instruction_t *decode_sdt(instruction_t *instr) {
  // the instruction to be decoded must be raw
  assert(instr->tag == RAW);

  uint32_t raw_data = instr->data.raw_data;

  instr->data.sdt.cond = extract_bits(raw_data, 28, 32);
  instr->data.sdt.is_offset = extract_bit(raw_data, 25);
  instr->data.sdt.is_preindexed = extract_bit(raw_data, 24);
  instr->data.sdt.load = extract_bit(raw_data, 20);
  instr->data.sdt.rn = extract_bits(raw_data, 16, 20);
  instr->data.sdt.rd = extract_bits(raw_data, 12, 16);
  instr->data.sdt.offset = extract_bits(raw_data, 0, 12);

  instr->tag = SDT;
  return instr;
}

/*
instruction_t *decode_branch(instruction_t *raw) {
  // the instruction to be decoded must be raw
  assert(raw->tag == RAW);
}

instruction_t *decode(instruction_t *raw) {
  // the instruction to be decoded must be raw
  assert(raw->tag == RAW);
}
*/
