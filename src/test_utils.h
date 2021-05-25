#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define RESET "\033[0m"
#define RED "\033[31m"    /* Red */
#define GREEN "\033[32m"  /* Green */
#define YELLOW "\033[33m" /* Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */

#define PASSED "OK"
#define FAILED "FAIL"


bool test_bool(bool cond, char *testname);
bool test_uint8(uint8_t expected, uint8_t got, char *testname);
bool test_uint16(uint16_t expected, uint16_t got, char *testname);
bool test_uint32(uint32_t expected, uint32_t got, char *testname);
bool test_string(char *expected, char *got, char *testname);
bool test_uint8_array(uint8_t *expected, uint8_t *got, size_t length,
                      char *testname);
void print_uint8_array(uint8_t *array, size_t length);

#endif /* TEST_UTILS_H */
