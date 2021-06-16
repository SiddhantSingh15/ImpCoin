#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <binn.h>
#include <sodium/utils.h>

#include "../lib/linked_list.h"
#include "../lib/transaction.h"
#include "../lib/block.h"
#include "../lib/utils.h"
#include "../lib/blockchain.h"
#include "test_utils.h"

bool test_bool(bool cond, char *testname) {
  printf("TEST - %s : %s%s%s\n", testname, cond ? GREEN : RED,
         cond ? PASSED : FAILED, NOCOLOUR);
  return cond;
}

bool test_uint8(uint8_t expected, uint8_t got, char *testname) {
  bool passed = expected == got;
  printf("TEST - %s : %s%s%s\n", testname, passed ? GREEN : RED,
         passed ? PASSED : FAILED, NOCOLOUR);

  if (!passed) {
    printf("\texpected : %d (0x%02x)\n", expected, expected);
    printf("\tgot      : %d (0x%02x)\n", got, got);
  }

  return passed;
}

bool test_uint16(uint16_t expected, uint16_t got, char *testname) {
  bool passed = expected == got;
  printf("TEST - %s : %s%s%s\n", testname, passed ? GREEN : RED,
         passed ? PASSED : FAILED, NOCOLOUR);

  if (!passed) {
    printf("\texpected : %d (0x%04x)\n", expected, expected);
    printf("\tgot      : %d (0x%04x)\n", got, got);
  }

  return passed;
}

bool test_uint32(uint32_t expected, uint32_t got, char *testname) {
  bool passed = expected == got;
  printf("TEST - %s : %s%s%s\n", testname, passed ? GREEN : RED,
         passed ? PASSED : FAILED, NOCOLOUR);

  if (!passed) {
    printf("\texpected : %d (0x%08x)\n", expected, expected);
    printf("\tgot      : %d (0x%08x)\n", got, got);
  }

  return passed;
}

bool test_string(char *expected, char *got, char *testname) {
  bool passed = expected == NULL ? got == NULL : strcmp(expected, got) == 0;
  printf("TEST - %s : %s%s%s\n", testname, passed ? GREEN : RED,
         passed ? PASSED : FAILED, NOCOLOUR);

  if (!passed) {
    printf("\texpected : %s\n", expected);
    printf("\tgot      : %s\n", got);
  }

  return passed;
}

bool test_uint8_array(uint8_t *expected, uint8_t *got, size_t length,
                      char *testname) {
  bool passed = true;
  for (int i = 0; i < length; i++) {
    passed = passed && (expected[i] == got[i]);
  }

  printf("TEST - %s : %s%s%s\n", testname, passed ? GREEN : RED,
         passed ? PASSED : FAILED, NOCOLOUR);

  if (!passed) {
    printf("\texpected : ");
    print_uint8_array(expected, length);
    printf("\n");
    printf("\tgot      : ");
    print_uint8_array(got, length);
    printf("\n");
  }

  return passed;
}

bool test_transaction(transaction *expected, transaction *got, char *testname) {
  char expected_buf[511];
  char got_buf[511];

  to_string_transaction(expected, expected_buf);
  to_string_transaction(got, got_buf);

  return test_string(expected_buf, got_buf, testname);
}

bool test_linked_list(linked_list *expected, linked_list *got,
                      void (*value_to_string)(void *, char *), char *testname) {

  char *expected_str = ll_to_string(expected, value_to_string);
  char *got_str = ll_to_string(got, value_to_string);

  bool passed = test_string(expected_str, got_str, testname);

  free(expected_str);
  free(got_str);

  return passed;
}

bool test_block(block *expected, block *got, char *testname) {

  char *expected_str = to_string_block(expected);
  char *got_str = to_string_block(got);

  bool passed = test_string(expected_str, got_str, testname);

  free(expected_str);
  free(got_str);

  return passed;
}

bool test_blockchain(blockchain *expected, blockchain *got, char *testname) {

  char *expected_str = blockchain_to_string(expected);
  char *got_str = blockchain_to_string(got);

  bool passed = test_string(expected_str, got_str, testname);

  free(expected_str);
  free(got_str);

  return passed;
}

bool test_hash(hash *expected, hash *got, char *testname) {

  char *expected_str = to_hex_string_hash(expected[0]);
  char *got_str = to_hex_string_hash(got[0]);

  bool passed = test_string(expected_str, got_str, testname);

  free(expected_str);
  free(got_str);

  return passed;
}

void print_uint8_array(uint8_t *array, size_t length) {
  for (int i = 0; i < length; i++) {
    printf("%02x ", array[i]);
  }
}

void track_test(bool test, int *passing, int *total) {
  (*total)++;
  if (test) {
    (*passing)++;
  }
}
