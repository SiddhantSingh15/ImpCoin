#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../global_helpers/definitions.h"
#include "../global_helpers/types.h"

// All parsing functions should look like this
// void parse_function (void* ll_node, union instr_code code, symbol_table* st);

void parse_dataproc(void *ll_node, union instr_code code, symbol_table *st) {
  assert (st == NULL);
  return;
}

void parse_mult(void *ll_node, union instr_code code, symbol_table *st) {
  assert (st == NULL);
  return;
}

void parse_sdt(void *ll_node, union instr_code code, symbol_table *st) {
  assert (st == NULL);
  return;
}

void parse_branch(void *ll_node, union instr_code code, symbol_table *st) {
  return;
}

// Should we have a 5th parser for LSL and ANDEQ?
// void parse_special(void *ll_node, instr_code code, symbol_table *st) {
//   return;
// }

