#include "decoder.h"
#include "emulate.h"
#include "emulate_utils.h"
#include "test_utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void test_decoder_halt(int *passing, int *total) {
  // tests for HALT
  instruction_t raw = {.tag = RAW, .data = {.raw_data = 0x0}};
  track_test(test_bool(HALT == decode(&raw)->tag, "Halt is decoded correctly"),
             passing, total);
}

void test_decoder_dataproc(int *total, int *passing) {
  // tests for DATAPROC
  const instruction_t expected = {.tag = DATAPROC,
                                  .data = {.dataproc = {.cond = 0xe,
                                                        .is_immediate = 0x1,
                                                        .opcode = 0xf,
                                                        .set_cond = 0x1,
                                                        .rn = 0xc,
                                                        .rd = 0x5,
                                                        .op2 = 0x08f}}};
  instruction_t raw = {.tag = RAW, .data = {.raw_data = 0xe3fc508f}};
  instruction_t *decoded = decode(&raw);
  dataproc_t expected_data = expected.data.dataproc;
  dataproc_t decoded_data = decoded->data.dataproc;

  track_test(test_uint32(expected.tag, decoded->tag,
                         "Dataproc tag is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.cond, decoded_data.cond,
                        "Dataproc cond is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.is_immediate, decoded_data.is_immediate,
                        "Dataproc is_immediate is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.opcode, decoded_data.opcode,
                        "Dataproc opcode is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.set_cond, decoded_data.set_cond,
                        "Dataproc set_cond is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.rn, decoded_data.rn,
                        "Dataproc rn is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.rd, decoded_data.rd,
                        "Dataproc rd is decoded correctly"),
             passing, total);
  track_test(test_uint16(expected_data.op2, decoded_data.op2,
                         "Dataproc op2 is decoded correctly"),
             passing, total);
}

void test_decoder_multiply(int *passing, int *total) {
  // tests for MULTIPLY
  const instruction_t expected = {.tag = MULTIPLY,
                                  .data = {.multiply = {.cond = 0xb,
                                                        .accumulate = 0x0,
                                                        .set_cond = 0x0,
                                                        .rd = 0xa,
                                                        .rn = 0xc,
                                                        .rs = 0x8,
                                                        .rm = 0x5}}};
  instruction_t raw = {.tag = RAW, .data = {.raw_data = 0xb00ac895}};
  instruction_t *decoded = decode(&raw);
  multiply_t expected_data = expected.data.multiply;
  multiply_t decoded_data = decoded->data.multiply;

  track_test(test_uint32(expected.tag, decoded->tag,
                         "Multiply tag is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.cond, decoded_data.cond,
                        "Multiply cond is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.accumulate, decoded_data.accumulate,
                        "Multiply accumulate is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.set_cond, decoded_data.set_cond,
                        "Multiply set_cond is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.rd, decoded_data.rd,
                        "Multiply rd is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.rn, decoded_data.rn,
                        "Multiply rn is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.rs, decoded_data.rs,
                        "Multiply rs is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.rm, decoded_data.rm,
                        "Multiply rm is decoded correctly"),
             passing, total);
}

void test_decoder_sdt(int *passing, int *total) {
  // tests for SDT
  const instruction_t expected = {.tag = SDT,
                                  .data = {.sdt = {.cond = 0xd,
                                                   .is_shift_R = 0x1,
                                                   .is_preindexed = 0x0,
                                                   .up_bit = 0x1,
                                                   .load = 0x0,
                                                   .rn = 0x5,
                                                   .rd = 0xe,
                                                   .offset = 0xa31}}};
  instruction_t raw = {.tag = RAW, .data = {.raw_data = 0xd685ea31}};
  instruction_t *decoded = decode(&raw);
  sdt_t expected_data = expected.data.sdt;
  sdt_t decoded_data = decoded->data.sdt;

  track_test(test_uint32(expected.tag, decoded->tag,
                         "Single Data Transfer tag is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.cond, decoded_data.cond,
                        "Single Data Transfer cond is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.is_shift_R, decoded_data.is_shift_R,
                        "Single Data Transfer is_shift_R is decoded correctly"),
             passing, total);
  track_test(
      test_uint8(expected_data.is_preindexed, decoded_data.is_preindexed,
                 "Single Data Transfer is_preindexed is decoded correctly"),
      passing, total);
  track_test(test_uint8(expected_data.up_bit, decoded_data.up_bit,
                        "Single Data Transfer up_bit is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.load, decoded_data.load,
                        "Single Data Transfer load is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.rn, decoded_data.rn,
                        "Single Data Transfer rn is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.rd, decoded_data.rd,
                        "Single Data Transfer rd is decoded correctly"),
             passing, total);
  track_test(test_uint16(expected_data.offset, decoded_data.offset,
                         "Single Data Transfer offset is decoded correctly"),
             passing, total);
}

void test_decoder_branch(int *passing, int *total) {
  // tests for BRANCH
  const instruction_t expected = {
      .tag = BRANCH, .data = {.branch = {.cond = 0x1, .offset = 0x463ce9}}};
  instruction_t raw = {.tag = RAW, .data = {.raw_data = 0x1a463ce9}};
  instruction_t *decoded = decode(&raw);
  branch_t expected_data = expected.data.branch;
  branch_t decoded_data = decoded->data.branch;

  track_test(test_uint32(expected.tag, decoded->tag,
                         "Branch tag is decoded correctly"),
             passing, total);
  track_test(test_uint8(expected_data.cond, decoded_data.cond,
                        "Branch cond is decoded correctly"),
             passing, total);
  track_test(test_uint16(expected_data.offset, decoded_data.offset,
                         "Branch offset is decoded correctly"),
             passing, total);
}

void test_decoder(int *passing, int *total) {
  printf("---------------------------------------------------------------------"
         "\n");
  printf("-----%sDECODER "
         "TESTS%s---------------------------------------------------\n",
         BOLDBLUE, RESET);
  int internal_passing = 0;
  int internal_total = 0;

  test_decoder_halt(&internal_passing, &internal_total);
  test_decoder_dataproc(&internal_passing, &internal_total);
  test_decoder_multiply(&internal_passing, &internal_total);
  test_decoder_sdt(&internal_passing, &internal_total);
  test_decoder_branch(&internal_passing, &internal_total);

  printf("---------------------------------------------------------------------"
         "\n");
  printf("%sPASSING: (%d/%d) tests%s\n",
         internal_passing == internal_total        ? GREEN
         : (internal_passing > internal_total / 2) ? YELLOW
                                                   : RED,
         internal_passing, internal_total, RESET);

  *passing = *passing + internal_passing;
  *total = *total + internal_total;
}

void test_to_uint8_array(int *passing, int *total) {
  uint8_t byte_array[WORD_SIZE_IN_BYTES * 4] = {0};
  uint32_t first = 0x1a463ce9;
  uint32_t second = 0xd685ea31;
  uint32_t third = 0x69420b00;
  uint32_t fourth = 0x69696969;

  to_uint8_array(first, byte_array);
  to_uint8_array(second, byte_array + 4);
  to_uint8_array(third, byte_array + 8);
  to_uint8_array(fourth, byte_array + 12);

  uint8_t expected_byte_array[WORD_SIZE_IN_BYTES * 4] = {
      0x1a, 0x46, 0x3c, 0xe9, 0xd6, 0x85, 0xea, 0x31,
      0x69, 0x42, 0x0b, 0x00, 0x69, 0x69, 0x69, 0x69};
  track_test(test_uint8_array(expected_byte_array, byte_array,
                              WORD_SIZE_IN_BYTES * 4,
                              "to_uint8_array works correctly"),
             passing, total);
}

void test_to_uint32(int *passing, int *total) {
  uint8_t byte_array[WORD_SIZE_IN_BYTES] = {0xd6, 0x85, 0xea, 0x31};
  track_test(test_uint32(0xd685ea31, to_uint32(byte_array),
                         "to_uint32 works correctly"),
             passing, total);
}

void test_unit_conversions(int *passing, int *total) {
  printf("---------------------------------------------------------------------"
         "\n");
  printf("-----%sUNIT CONVERSION "
         "TESTS%s-------------------------------------------\n",
         BOLDBLUE, RESET);
  int internal_passing = 0;
  int internal_total = 0;

  test_to_uint8_array(&internal_passing, &internal_total);
  test_to_uint32(&internal_passing, &internal_total);

  printf("---------------------------------------------------------------------"
         "\n");
  printf("%sPASSING: (%d/%d) tests%s\n",
         internal_passing == internal_total        ? GREEN
         : (internal_passing > internal_total / 2) ? YELLOW
                                                   : RED,
         internal_passing, internal_total, RESET);

  *passing = *passing + internal_passing;
  *total = *total + internal_total;
}

int main(void) {
  int passing = 0;
  int total = 0;
  test_decoder(&passing, &total);
  test_unit_conversions(&passing, &total);
  printf(
      "%s---------------------------------------------------------------------%"
      "s\n",
      BOLDWHITE, RESET);
  printf("%sTOTAL PASSING: (%d/%d) tests%s\n",
         passing == total        ? BOLDGREEN
         : (passing > total / 2) ? BOLDYELLOW
                                 : BOLDRED,
         passing, total, RESET);
  printf(
      "%s---------------------------------------------------------------------%"
      "s\n",
      BOLDWHITE, RESET);
  return EXIT_SUCCESS;
}