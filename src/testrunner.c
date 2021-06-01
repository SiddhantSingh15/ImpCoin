#include "emulate_lib/decoder.h"
#include "global_helpers/types.h"
#include "emulate_lib/emulate_utils.h"
#include "emulate_lib/exec_utils.h"
#include "testing/test_utils.h"
#include "assemble_lib/symbol_table_utils.h"
#include <limits.h>
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
         BOLDBLUE, NOCOLOUR);
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
         internal_passing, internal_total, NOCOLOUR);

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
      0xe9, 0x3c, 0x46, 0x1a, 0x31, 0xea, 0x85, 0xd6,
      0x00, 0x0b, 0x42, 0x69, 0x69, 0x69, 0x69, 0x69};
  track_test(test_uint8_array(expected_byte_array, byte_array,
                              WORD_SIZE_IN_BYTES * 4,
                              "to_uint8_array works correctly"),
             passing, total);
}


void test_to_uint32_reg(int *passing, int *total) {
  uint8_t byte_array[WORD_SIZE_IN_BYTES] = {0x31, 0xea, 0x85, 0xd6};
  track_test(test_uint32(0xd685ea31, to_uint32_reg(byte_array),
                         "to_uint32_reg works correctly"),
             passing, total);
}


void test_to_uint32_print(int *passing, int *total) {
  uint8_t byte_array[WORD_SIZE_IN_BYTES] = {0xd6, 0x85, 0xea, 0x31};
  track_test(test_uint32(0xd685ea31, to_uint32_print(byte_array),
                         "to_uint32_print works correctly"),
             passing, total);
}


void test_overflow(int *passing, int *total) {
  track_test(test_bool(!overflow(2, 2), "2 + 2 doesn't overflow"), passing,
             total);
  track_test(test_bool(!overflow(-2, -2), "-2 + -2 doesn't overflow"), passing,
             total);
  track_test(test_bool(overflow(INT_MAX, 1000), "INT_MAX + 1000 overflows"),
             passing, total);
  track_test(test_bool(!overflow(69, -69), "69 + -69 doesn't overflow"),
             passing, total);
  track_test(test_bool(!overflow(-420, 420), "-420 + 420 doesn't overflow"),
             passing, total);
      track_test(
          test_bool(overflow(INT_MIN, -6969), "INT_MIN - 6969 overflows"),
          passing, total);
}


void test_bit_operations(int *passing, int *total) {
  printf("---------------------------------------------------------------------"
         "\n");
  printf("-----%sBIT OPERATIONS "
         "TESTS%s-------------------------------------------\n",
         BOLDBLUE, NOCOLOUR);
  int internal_passing = 0;
  int internal_total = 0;

  test_to_uint8_array(&internal_passing, &internal_total);
  test_to_uint32_reg(&internal_passing, &internal_total);
  test_to_uint32_print(&internal_passing, &internal_total);
  test_overflow(&internal_passing, &internal_total);

  printf("---------------------------------------------------------------------"
         "\n");
  printf("%sPASSING: (%d/%d) tests%s\n",
         internal_passing == internal_total        ? GREEN
         : (internal_passing > internal_total / 2) ? YELLOW
                                                   : RED,
         internal_passing, internal_total, NOCOLOUR);

  *passing = *passing + internal_passing;
  *total = *total + internal_total;
}

void test_symbol_table_insert(int *passing, int *total) {

  symbol_table *st = init_symbol_table();

  insert_to_symbol_table(st, "first", 5);
  insert_to_symbol_table(st, "second", 10);
  insert_to_symbol_table(st, "third", 11);
  insert_to_symbol_table(st, "fourth", 102);
  insert_to_symbol_table(st, "fifth", 30);
  uint32_t *first = retrieve_address(st, "first");
  uint32_t *second = retrieve_address(st, "second");
  uint32_t *third = retrieve_address(st, "third");
  uint32_t *fourth = retrieve_address(st, "fourth");
  uint32_t *fifth = retrieve_address(st, "fifth");

  track_test(
    test_bool(
      *first == 5,
      "First Insertion Works Correctly"
    ), passing, total);
  track_test(
    test_bool(
      *second == 10,
      "Second Insertion Works Correctly"
    ), passing, total);
  track_test(
    test_bool(
      *third == 11,
      "Third Insertion Works Correctly"
    ), passing, total);
  track_test(
    test_bool(
      *fourth == 102,
      "Fourth Insertion Works Correctly"
    ), passing, total);
  track_test(
    test_bool(
      *fifth == 30,
      "Fifth Insertion Works Correctly"
    ), passing, total);
}

void test_symbol_table(int *passing, int*total) {
  printf("---------------------------------------------------------------------"
         "\n");
  printf("-----%sSYMBOL TABLE "
         "TESTS%s----------------------------------------------\n",
         BOLDBLUE, NOCOLOUR);
  int internal_passing = 0;
  int internal_total = 0;

  test_symbol_table_insert(&internal_passing, &internal_total);

  printf("---------------------------------------------------------------------"
         "\n");
  printf("%sPASSING: (%d/%d) tests%s\n",
         internal_passing == internal_total        ? GREEN
         : (internal_passing > internal_total / 2) ? YELLOW
                                                   : RED,
         internal_passing, internal_total, NOCOLOUR);

  *passing = *passing + internal_passing;
  *total = *total + internal_total;
}

int main(void) {
  int passing = 0;
  int total = 0;
  test_decoder(&passing, &total);
  test_bit_operations(&passing, &total);
  test_symbol_table(&passing, &total);
  printf(
      "%s---------------------------------------------------------------------%"
      "s\n",
      BOLDWHITE, NOCOLOUR);
  printf("%sTOTAL PASSING: (%d/%d) tests%s\n",
         passing == total        ? BOLDGREEN
         : (passing > total / 2) ? BOLDYELLOW
                                 : BOLDRED,
         passing, total, NOCOLOUR);
  printf(
      "%s---------------------------------------------------------------------%"
      "s\n",
      BOLDWHITE, NOCOLOUR);
  return EXIT_SUCCESS;
}
