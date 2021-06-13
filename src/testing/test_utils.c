#include "test_utils.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
