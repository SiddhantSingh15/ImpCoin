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

uint32_t construct_dataproc_binary (dataproc_t *instr) {
  // Base - 0000 0000 0000 0000 0000 0000 0000 0000
  uint32_t dataproc_base = 0x00000000;
  dataproc_base |= instr->op2 << OP2_POS;
  dataproc_base |= instr->is_immediate << I_POS;
  dataproc_base |= instr->rd << RD_POS;
  dataproc_base |= instr->rn << RN_POS;
  dataproc_base |= instr->set_cond << S_POS;
  dataproc_base |= instr->opcode << OPCODE_POS;
  dataproc_base |= instr->cond << COND_POS;
  return dataproc_base;
}

uint32_t construct_mult_binary (multiply_t *instr) {
  // Base - 0000 0000 0000 0000 0000 0000 1001 0000
  uint32_t mult_base = 0x00000090;
  mult_base |= instr->rm << RM_POS;
  mult_base |= instr->rs << RS_POS;
  mult_base |= instr->rn << RN_POS;
  mult_base |= instr->rd << RD_POS;
  mult_base |= instr->set_cond << S_POS;
  mult_base |= instr->accumulate << A_POS;
  mult_base |= instr->cond << COND_POS;
  return mult_base;
}

uint32_t construct_sdt_binary (sdt_t *instr) {
  // Base - 0000 0100 0000 0000 0000 0000 0000 0000
  uint32_t sdt_base = 0x04000000;
  sdt_base |= instr->offset << OFFSET_POS;
  sdt_base |= instr->rd << RD_POS;
  sdt_base |= instr->rn << RN_POS;
  sdt_base |= instr->load << L_POS;
  sdt_base |= instr->up_bit << U_POS;
  sdt_base |= instr->is_preindexed << P_POS;
  sdt_base |= instr->is_shift_R << I_POS;
  sdt_base |= instr->cond << COND_POS;
  return sdt_base;
}

uint32_t construct_branch_binary (branch_t *instr) {
  // Base - 0000 1010 0000 0000 0000 0000 0000 0000
  uint32_t branch_base = 0x0A000000;
  branch_base |= instr->offset << OFFSET_POS;
  branch_base |= instr->cond << COND_POS;
  return branch_base;
}

uint32_t parse_dataproc(void *ll_node, union instr_code code, symbol_table *st) {
  assert (st == NULL);
  dataproc_t dataproc_instr = {0};
  node *node = ll_node;
  uint32_t line = node->address;
  token_list *tokens = node->value;
  if (code.dataproc_opcode <= 0xA && code.dataproc_opcode >= 0x8) {
    // Check for TST, TEQ, CMP
    // 0 - INSTR, 1 - RD, 2  - <#expression>
    dataproc_instr.set_cond = SET;
    assert_token(tokens->list[1].type == REG, 1, line);
    dataproc_instr.rn = tokens->list[1].data;
    if (tokens->list[2].type == SEPARATOR) {
      assert_token(tokens->list[2].data == '#');
      dataproc_instr.is_immediate = SET;
    }
    dataproc_instr.op2 = parse_operand2(tokens, 2, line);
  } else if (code.dataproc_opcode == 0xD) {
    // Check for MOV
    // 0 - INSTR, 1 - RD, 2  - <#expression>
    assert_token(tokens->list[1].type == REG, 1, line);
    dataproc_instr.rd = tokens->list[1].data;
    if (tokens->list[2].type == SEPARATOR) {
      assert_token(tokens->list[2].data == '#');
      dataproc_instr.is_immediate = SET;
    }
    dataproc_instr.op2 = parse_operand2(tokens, 2, line);
  } else {
    // Remaining can only be AND, EOR, SUB, RSB, ADD, ORR
    // 0 - INSTR, 1 - RD, 2 - RN, 3 - <#expression>
    assert_token(tokens->list[1].type == REG, 1, line);
    dataproc_instr.rd = tokens->list[1].data;
    assert_token(tokens->list[2].type == REG, 2, line);
    dataproc_instr.rn = tokens->list[2].data;
    if (tokens->list[3].type == SEPARATOR) {
      assert_token(tokens->list[2].data == '#');
      dataproc_instr.is_immediate = SET;
    }
    dataproc_instr.op2 = parse_operand2(tokens, 3, line);
  }
  dataproc_instr.opcode = code.dataproc_opcode;
  dataproc_instr.cond = AL;

  return construct_dataproc_binary(dataproc_instr);
}

uint32_t parse_mult(void *ll_node, union instr_code code, symbol_table *st) {
  assert (st == NULL);
  multiply_t mult_instr = {0};
  return 0;
}

uint32_t parse_sdt(void *ll_node, union instr_code code, symbol_table *st) {
  assert (st == NULL);
  sdt_t sdt_instr = {0};
  return 0;
}

uint32_t parse_branch(void *ll_node, union instr_code code, symbol_table *st) {
  branch_t branch_instr = {0};
  node *node = ll_node;
  token_list *tokens = node->value;
  branch_instr.cond = code.branch_cond;
  for (int i = 0; i < tokens->size; i++) {
    if (tokens->list[i].type == LABEL) {
      uint32_t *label_address = (uint32_t*) st_retrieve(st, tokens->list[i].data.label);
      branch_instr.offset = node->address - *label_address;
    }
  }

  // Encode
  return construct_branch_binary(&branch_instr);
}

// Should we have a 5th parser for LSL and ANDEQ?
// void parse_special(void *ll_node, instr_code code, symbol_table *st) {
//   return;
// }
