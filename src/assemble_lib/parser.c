#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tokenizer.h"
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
  branch_t *branch_instr = calloc(1, sizeof(branch_t));
  node *node = ll_node;
  token_list *tokens = node->value;
  branch_instr->cond = code.branch_cond;
  for (int i = 0; i < tokens->size; i++) {
    if (tokens->list[i].type == LABEL) {
      uint32_t *label_address = (uint32_t*) st_retrieve(st, tokens->list[i].data.label);
      branch_instr->offset = node->address - *label_address;
    }
  }
  free(node->value);

  // Encode

  // Write
}

// Should we have a 5th parser for LSL and ANDEQ?
// void parse_special(void *ll_node, instr_code code, symbol_table *st) {
//   return;
// }

