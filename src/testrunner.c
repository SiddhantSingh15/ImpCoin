#include "decoder.h"
#include "emulate.h"
#include "test_utils.h"
#include <stdlib.h>

void test_decoder_halt(void) {
  // tests for HALT
  instruction_t raw_halt = {.tag = RAW, .data = {.raw_data = 0x0}};
  test_bool(HALT == decode(&raw_halt)->tag, "Halt is decoded correctly");
}

void test_decoder_dataproc(void) {
  // tests for DATAPROC
  const instruction_t dataproc_expected = {
      .tag = DATAPROC,
      .data = {.dataproc = {.cond = 0xe,
                            .is_immediate = 0x1,
                            .opcode = 0xf,
                            .set_cond = 0x1,
                            .rn = 0xc,
                            .rd = 0x5,
                            .op2 = 0x08f}}};
  instruction_t dataproc_raw = {.tag = RAW, .data = {.raw_data = 0xe3fc508f}};
  instruction_t *dataproc_decoded = decode(&dataproc_raw);

  test_uint8(dataproc_expected.data.dataproc.cond,
             dataproc_decoded->data.dataproc.cond,
             "Dataproc cond is decoded correctly");
  test_uint8(dataproc_expected.data.dataproc.is_immediate,
             dataproc_decoded->data.dataproc.is_immediate,
             "Dataproc is_immediate is decoded correctly");
  test_uint8(dataproc_expected.data.dataproc.opcode,
             dataproc_decoded->data.dataproc.opcode,
             "Dataproc opcode is decoded correctly");
  test_uint8(dataproc_expected.data.dataproc.set_cond,
             dataproc_decoded->data.dataproc.set_cond,
             "Dataproc set_cond is decoded correctly");
  test_uint8(dataproc_expected.data.dataproc.rn,
             dataproc_decoded->data.dataproc.rn,
             "Dataproc rn is decoded correctly");
  test_uint8(dataproc_expected.data.dataproc.rd,
             dataproc_decoded->data.dataproc.rd,
             "Dataproc rd is decoded correctly");
  test_uint16(dataproc_expected.data.dataproc.op2,
              dataproc_decoded->data.dataproc.op2,
              "Dataproc op2 is decoded correctly");
}

void test_decoder_multiply(void) {
  // tests for MULTIPLY
  const instruction_t multiply_expected = {
      .tag = MULTIPLY,
      .data = {.multiply = {.cond = 0xb,
                            .accumulate = 0x0,
                            .set_cond = 0x0,
                            .rd = 0xa,
                            .rn = 0xc,
                            .rs = 0x8,
                            .rm = 0x5}}};
  instruction_t multiply_raw = {.tag = RAW, .data = {.raw_data = 0xb00ac895}};
  instruction_t *multiply_decoded = decode(&multiply_raw);

  test_uint8(multiply_expected.data.multiply.cond,
             multiply_decoded->data.multiply.cond,
             "Multiply cond is decoded correctly");
  test_uint8(multiply_expected.data.multiply.accumulate,
             multiply_decoded->data.multiply.accumulate,
             "Multiply accumulate is decoded correctly");
  test_uint8(multiply_expected.data.multiply.set_cond,
             multiply_decoded->data.multiply.set_cond,
             "Multiply set_cond is decoded correctly");
  test_uint8(multiply_expected.data.multiply.rd,
             multiply_decoded->data.multiply.rd,
             "Multiply rd is decoded correctly");
  test_uint8(multiply_expected.data.multiply.rn,
             multiply_decoded->data.multiply.rn,
             "Multiply rn is decoded correctly");
  test_uint8(multiply_expected.data.multiply.rs,
             multiply_decoded->data.multiply.rs,
             "Multiply rs is decoded correctly");
  test_uint8(multiply_expected.data.multiply.rm,
             multiply_decoded->data.multiply.rm,
             "Multiply rm is decoded correctly");
}

void test_decoder_sdt(void) {
  // tests for SDT
  const instruction_t sdt_expected = {.tag = SDT,
                                      .data = {.sdt = {.cond = 0xd,
                                                       .is_shift_R = 0x1,
                                                       .is_preindexed = 0x0,
                                                       .up_bit = 0x1,
                                                       .load = 0x0,
                                                       .rn = 0x5,
                                                       .rd = 0xe,
                                                       .offset = 0xa31}}};
  instruction_t sdt_raw = {.tag = RAW, .data = {.raw_data = 0xd685ea31}};
  instruction_t *sdt_decoded = decode(&sdt_raw);

  test_uint8(sdt_expected.data.sdt.cond, sdt_decoded->data.sdt.cond,
             "Single Data Transfer cond is decoded correctly");
  test_uint8(sdt_expected.data.sdt.is_shift_R, sdt_decoded->data.sdt.is_shift_R,
             "Single Data Transfer is_shift_R is decoded correctly");
  test_uint8(sdt_expected.data.sdt.is_preindexed,
             sdt_decoded->data.sdt.is_preindexed,
             "Single Data Transfer is_preindexed is decoded correctly");
  test_uint8(sdt_expected.data.sdt.up_bit, sdt_decoded->data.sdt.up_bit,
             "Single Data Transfer up_bit is decoded correctly");
  test_uint8(sdt_expected.data.sdt.load, sdt_decoded->data.sdt.load,
             "Single Data Transfer load is decoded correctly");
  test_uint8(sdt_expected.data.sdt.rn, sdt_decoded->data.sdt.rn,
             "Single Data Transfer rn is decoded correctly");
  test_uint8(sdt_expected.data.sdt.rd, sdt_decoded->data.sdt.rd,
             "Single Data Transfer rd is decoded correctly");
  test_uint16(sdt_expected.data.sdt.offset, sdt_decoded->data.sdt.offset,
              "Single Data Transfer offset is decoded correctly");
}

void test_decoder_branch(void) {
  // tests for BRANCH
  const instruction_t branch_expected = {
      .tag = SDT, .data = {.branch = {.cond = 0x1, .offset = 0x463ce9}}};
  instruction_t branch_raw = {.tag = RAW, .data = {.raw_data = 0x1a463ce9}};
  instruction_t *branch_decoded = decode(&branch_raw);

  test_uint8(branch_expected.data.branch.cond, branch_decoded->data.branch.cond,
             "Branch cond is decoded correctly");
  test_uint16(branch_expected.data.branch.offset,
              branch_decoded->data.branch.offset,
              "Branch offset is decoded correctly");
}

void test_decoder(void) {
  test_decoder_halt();
  test_decoder_dataproc();
  test_decoder_multiply();
  test_decoder_sdt();
  test_decoder_branch();
}

int main(void) {
  test_decoder();
  return EXIT_SUCCESS;
}
