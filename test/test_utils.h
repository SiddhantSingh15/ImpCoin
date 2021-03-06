#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
bool test_transaction(transaction *expected, transaction *got, char *testname);
bool test_linked_list(linked_list *expected, linked_list *got,
                      void (*value_to_string)(void *, char *), char *testname);
bool test_block(block *expected, block *got, char *testname);
bool test_blockchain(blockchain *expected, blockchain *got, char *testname);
bool test_hash(hash *expected, hash *got, char *testname);
void track_test(bool test, int *passing, int *total);

#endif /* TEST_UTILS_H */
