#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tokenizer.h"
#include "../global_helpers/definitions.h"
#include "../global_helpers/types.h"
#include "tokens.h"

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

void throw_parse_error(uint8_t pos, uint32_t line) {
  printf("Parse error: invalid token at position %d on line %d", pos, line);
  exit(1);
}

void assert_token(bool token_cond, uint8_t pos, uint32_t line) {
  if (!token_cond) {
    throw_parse_error(pos, line);
  }
}

uint16_t parse_operand2(token_list *tokens, uint8_t *index, uint8_t line) {
  uint8_t i = *index;
  // If op2 is an immediate value ('#' identified)
  if (tokens->list[i].type == SEPARATOR) {
    assert_token(tokens->list[i].data.separator == '#', i, line);
    assert_token(tokens->list[i + 1].type == EXPRESSION, i + 1, line);
    *index = i + 2;
    // Set up rotate byte
    // Rotation example
    // 1. 00 00 xx 00 -> rotate = 16 - 2
    // 2. 00 00 0x x0 -> rotate = 16 - 1
    // 3. 00 00 00 xx -> no rotation
    uint16_t rotate_count = 16;
    // Rotate only if it is required - If value is less than 0xFF,
    // no rotation is needed
    if (tokens->list[i + 1].data.exp > 0xFF) {
      // 0x3 = 11 in binary, this while loop checks to see if the last  any of the
      // 2 bits are set
      // E.G. 1100 is not valid, 0011 is valid, 0110 is valid
      while (!(tokens->list[i + 1].data.exp & 0x3)) {
        rotate_count--;
        tokens->list[i + 1].data.exp >>= SHIFT_MULTIPLIER;
      }
    }
    rotate_count <<= ONE_B;
    return rotate_count | tokens->list[i + 1].data.exp;
  }
  // op2 is not immediate, first token will be a register
  assert_token(tokens->list[i].type == REG, i, line);
  uint8_t rm = tokens->list[i].data.reg;

  // No shift present
  if (tokens->size == i || tokens->list[i + 1].type != SHIFTNAME) {
    *index = i + 1;
    return (uint16_t) rm;
  }

  // Shift present
  assert_token(tokens->list[i + 1].type == SHIFTNAME, i + 1, line);
  uint8_t shift = tokens->list[i + 1].data.shift_name << 1;
  assert(shift < 4);

  // Structure of shift
  // 1. <shiftname> <#expression> - 3 tokens
  // 2. <shiftname> <register> - 2 tokens
  if (tokens->list[i + 2].type == SEPARATOR) {
    // Case 1: <shiftname> <register> - 3 tokens
    assert_token(tokens->list[i + 2].data.separator == '#', i + 2, line);
    assert_token(tokens->list[i + 3].type == EXPRESSION, i + 3, line);
    shift |= tokens->list[i + 3].data.exp << 3;
    *index = i + 4;
  } else {
    // Case 2: <shiftname> <#expression> - 2 tokens
    assert_token(tokens->list[i + 2].type == REG, i + 2, line);
    shift |= tokens->list[i + 2].data.reg << 4;
    shift++;
    *index = i + 3;
  }
  uint16_t base = 0;
  base |= rm;
  base |= shift << 4;
  return base;
}

uint32_t parse_dataproc(void *ll_node, union instr_code code, symbol_table *st) {
  dataproc_t dataproc_instr = {0};
  node *node = ll_node;
  uint32_t line = node->address;
  token_list *tokens = node->value;
  uint8_t currptr;
  if (code.dataproc_opcode <= 0xA && code.dataproc_opcode >= 0x8) {
    // Check for TST, TEQ, CMP
    // 0 - INSTR, 1 - RD, 2  - <#expression>
    dataproc_instr.set_cond = SET;
    assert_token(tokens->list[1].type == REG, 1, line);
    dataproc_instr.rn = tokens->list[1].data.reg;
    if (tokens->list[2].type == SEPARATOR) {
      assert_token(tokens->list[2].data.separator == '#', 2, line);
      dataproc_instr.is_immediate = SET;
    }
    currptr = 2;
    dataproc_instr.op2 = parse_operand2(tokens, &currptr, line);
  } else if (code.dataproc_opcode == 0xD) {
    // Check for MOV
    // 0 - INSTR, 1 - RD, 2  - <#expression>
    assert_token(tokens->list[1].type == REG, 1, line);
    dataproc_instr.rd = tokens->list[1].data.reg;
    if (tokens->list[2].type == SEPARATOR) {
      assert_token(tokens->list[2].data.separator == '#', 2, line);
      dataproc_instr.is_immediate = SET;
    }
    currptr = 2;
    dataproc_instr.op2 = parse_operand2(tokens, &currptr, line);
  } else {
    // Remaining can only be AND, EOR, SUB, RSB, ADD, ORR
    // 0 - INSTR, 1 - RD, 2 - RN, 3 - <#expression>
    assert_token(tokens->list[1].type == REG, 1, line);
    dataproc_instr.rd = tokens->list[1].data.reg;
    assert_token(tokens->list[2].type == REG, 2, line);
    dataproc_instr.rn = tokens->list[2].data.reg;
    if (tokens->list[3].type == SEPARATOR) {
      assert_token(tokens->list[3].data.separator == '#', 3, line);
      dataproc_instr.is_immediate = SET;
    }
    currptr = 3;
    dataproc_instr.op2 = parse_operand2(tokens, &currptr, line);
  }
  dataproc_instr.opcode = code.dataproc_opcode;
  dataproc_instr.cond = AL;

  return construct_dataproc_binary(&dataproc_instr);
}

uint32_t parse_mult(void *ll_node, union instr_code code, symbol_table *st) {
  multiply_t mult_instr = {0};
  node *node = ll_node;
  token_list *tokens = node->value;
  uint32_t line = node->address;

  mult_instr.cond = AL;

  assert_token(tokens->list[1].type == REG, 1, line);
  mult_instr.rd = tokens->list[1].data.reg;

  assert_token(tokens->list[2].type == REG, 2, line);
  mult_instr.rm = tokens->list[2].data.reg;

  assert_token(tokens->list[3].type == REG, 3, line);
  mult_instr.rs = tokens->list[3].data.reg;

  if (tokens->size == 5) {
    assert_token(tokens->list[4].type == REG, 4, line);
    mult_instr.rn = tokens->list[4].data.reg;
  }

  mult_instr.accumulate = code.mul_a;

  mult_instr.set_cond = !SET;

  return construct_mult_binary(&mult_instr);
}

uint32_t parse_sdt(void *ll_node, union instr_code code, symbol_table *st) {
  sdt_t sdt_instr = {0};
  node *node = ll_node;
  token_list *tokens = node->value;
  uint32_t line = node->address;

  sdt_instr.cond = AL;
  sdt_instr.load = code.sdt_l;

  assert_token(tokens->list[1].type == REG, 1, line);
  sdt_instr.rd = tokens->list[1].data.reg;

  assert_token(tokens->list[2].type == SEPARATOR, 2, line);
  if (tokens->list[2].data.separator == '=') {
    assert_token(strcmp(tokens->list[0].data.instr_name, "ldr") == 0, 2, line);
    assert_token(tokens->list[3].type == EXPRESSION, 3, line);
    // TODO: constant expressions
  }

  assert_token(tokens->list[3].data.separator == '[', 3, line);
  assert_token(tokens->list[4].type == REG, 4, line);
  sdt_instr.rn = tokens->list[4].data.reg;

  // Check post / pre-indexed
  if (tokens->list[5].type == SEPARATOR &&
      tokens->list[5].data.separator == ']') {
    sdt_instr.is_preindexed = (tokens->size == 6);
  }

  // No offset
  if (tokens->size == 6) {
    sdt_instr.is_shift_R = !SET;
    sdt_instr.offset = 0;
    sdt_instr.up_bit = !SET;
    return construct_sdt_binary(&sdt_instr);
  }

  uint8_t currptr;

  if (tokens->list[5].type == SEPARATOR) {
    sdt_instr.up_bit = (tokens->list[5].data.separator == '-') ? !SET : SET;
    sdt_instr.is_shift_R = (tokens->list[5].data.separator == '#') ? !SET : SET;
    currptr = (tokens->list[5].data.separator == '#') ? 5 : 6;
    sdt_instr.offset = parse_operand2(tokens, &currptr, line);
  } else {
    sdt_instr.is_shift_R = SET;
    sdt_instr.up_bit = SET;
    sdt_instr.offset = parse_operand2(tokens, &currptr, line);
  }

  if (sdt_instr.is_preindexed) {
    assert_token(tokens->list[currptr].type == SEPARATOR, currptr, line);
    assert_token(tokens->list[currptr].data.separator == ']', currptr, line);
  }

  return construct_sdt_binary(&sdt_instr);
}

uint32_t parse_branch(void *ll_node, union instr_code code, symbol_table *st) {
  node *node = ll_node;
  token_list *tokens = node->value;
  assert_token(tokens->list[1].type == '[', 1, node->address);
  branch_t branch_instr = {0};
  branch_instr.cond = code.branch_cond;
  uint32_t *label_address = (uint32_t*) st_retrieve(st, tokens->list[1].data.label);
  branch_instr.offset = node->address - *label_address;
  return construct_branch_binary(&branch_instr);
}

// Should we have a 5th parser for LSL and ANDEQ?
// void parse_special(void *ll_node, instr_code code, symbol_table *st) {
//   return;
// }
