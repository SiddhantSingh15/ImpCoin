#include "../global_helpers/definitions.h"
#include "../global_helpers/types.h"
#include "tokenizer.h"
#include "tokens.h"
#include "linked_list.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// All parsing functions should look like this
// void parse_function (void* ll_node, union instr_code code, symbol_table* st);

uint32_t construct_dataproc_binary(dataproc_t *instr) {
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

uint32_t construct_mult_binary(multiply_t *instr) {
  // Base - 0000 0000 0000 0000 0000 0000 1001 0000
  uint32_t mult_base = 0x00000090;
  mult_base |= instr->rm << RM_POS;
  mult_base |= instr->rs << RS_POS;
  mult_base |= instr->rn << MULTIPLY_RN_POS;
  mult_base |= instr->rd << MULTIPLY_RD_POS;
  mult_base |= instr->set_cond << S_POS;
  mult_base |= instr->accumulate << A_POS;
  mult_base |= instr->cond << COND_POS;
  return mult_base;
}

uint32_t construct_sdt_binary(sdt_t *instr) {
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

uint32_t construct_branch_binary(branch_t *instr) {
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

bool assert_token(bool token_cond, uint8_t pos, uint32_t line) {
  if (!token_cond) {
    throw_parse_error(pos, line);
    return false;
  }
  return true;
}

bool assert_separator(struct token tok, char separator, uint8_t pos,
                      uint8_t line) {
  if (tok.type != SEPARATOR || tok.data.separator != separator) {
    throw_parse_error(pos, line);
    return false;
  }
  return true;
}

bool cond_move(bool cond, uint8_t *pos) {
  if (cond) {
    (*pos)++;
  }
  return cond;
}

bool match_separator(struct token tok, char separator) {
  if (tok.type == SEPARATOR && tok.data.separator == separator) {
    return true;
  }
  return false;
}

union token_data get_and_move(struct token tok, enum token_type expected,
                               uint8_t *pos, uint8_t line){
    cond_move(assert_token(tok.type == expected, *pos, line), pos);;
    return tok.data;
}

union token_data match_and_get(struct token tok, enum token_type expected,
                               uint8_t pos, uint8_t line){
    assert_token(tok.type == expected, pos, line);
    return tok.data;
}

uint16_t parse_operand2(token_list *tokens, uint8_t *curr_token, uint8_t line) {
  uint8_t i = *curr_token;
  // If op2 is an immediate value (EXPR identified)
  if (tokens->list[i].type == EXPRESSION) {
    // Set up rotate byte
    // Rotation example
    // 1. 00 00 xx 00 -> rotate = 16 - 2
    // 2. 00 00 0x x0 -> rotate = 16 - 1
    // 3. 00 00 00 xx -> no rotation
    uint16_t rotate_count = 16;
    // Rotate only if it is required - If value is less than 0xFF,
    // no rotation is needed
    if (tokens->list[i].data.exp > 0xFF) {
      // 0x3 = 11 in binary, this while loop checks to see if the last  any of
      // the 2 bits are set E.G. 1100 is not valid, 0011 is valid, 0110 is valid
      while (!(tokens->list[i].data.exp & 0x3)) {
        rotate_count--;
        tokens->list[i].data.exp >>= SHIFT_MULTIPLIER;
      }
    }
    rotate_count <<= ONE_B;
    *curr_token = i + 1;
    return rotate_count | tokens->list[i].data.exp;
  }
  // op2 is not immediate, first token will be a register
  uint8_t rm = match_and_get(tokens->list[i], REG, i, line).reg;

  // No shift present
  if (tokens->size == i || tokens->list[i + 1].type != SHIFTNAME) {
    *curr_token = i + 1;
    return (uint16_t)rm;
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
    *curr_token = i + 4;
  } else {
    // Case 2: <shiftname> <#expression> - 2 tokens
    assert_token(tokens->list[i + 2].type == REG, i + 2, line);
    shift |= tokens->list[i + 2].data.reg << 4;
    shift++;
    *curr_token = i + 3;
  }
  uint16_t base = 0;
  base |= rm;
  base |= shift << 4;
  return base;
}

uint32_t parse_dataproc(void *ll_node, union instr_code code,
                        symbol_table *st) {

  if (code.dataproc_opcode == ANDEQ) {
    return 0;
  }

  dataproc_t instr = {0};
  node *node = ll_node;
  token_list *tokens = node->value;

  uint32_t line = (node->address / 4) + 1;
  uint8_t pos = 1;

  instr.opcode = code.dataproc_opcode;
  instr.cond = AL;

  if (code.dataproc_opcode <= 0xA && code.dataproc_opcode >= 0x8) {
    // Check for TST, TEQ, CMP
    // 0 - INSTR, 1 - RD, 2  - <#expression>
    instr.set_cond = SET;
    instr.rn = get_and_move(tokens->list[pos], REG, &pos, line).reg;
    instr.is_immediate =
        cond_move(match_separator(tokens->list[pos], '#'), &pos);

    instr.op2 = parse_operand2(tokens, &pos, line);
    return construct_dataproc_binary(&instr);
  }

  if (code.dataproc_opcode == MOV) {
    // Check for MOV
    // 0 - INSTR, 1 - RD, 2  - <#expression>
    instr.rd = get_and_move(tokens->list[pos], REG, &pos, line).reg;
    instr.is_immediate =
        cond_move(match_separator(tokens->list[pos], '#'), &pos);

    instr.op2 = parse_operand2(tokens, &pos, line);
    return construct_dataproc_binary(&instr);
  }

  // Remaining can only be AND, EOR, SUB, RSB, ADD, ORR
  // 0 - INSTR, 1 - RD, 2 - RN, 3 - <#expression>
  instr.rd = get_and_move(tokens->list[pos], REG, &pos, line).reg;
  instr.rn = get_and_move(tokens->list[pos], REG, &pos, line).reg;
  instr.is_immediate = cond_move(match_separator(tokens->list[pos], '#'), &pos);

  instr.op2 = parse_operand2(tokens, &pos, line);
  return construct_dataproc_binary(&instr);
}

// TODO: change this to pos, with get_and_move?
uint32_t parse_mult(void *ll_node, union instr_code code, symbol_table *st) {
  node *node = ll_node;
  token_list *tokens = node->value;
  uint32_t line = (node->address / 4) + 1;

  multiply_t mult_instr = {
      .cond = AL,
      .rd = match_and_get(tokens->list[1], REG, 1, line).reg,
      .rm = match_and_get(tokens->list[2], REG, 2, line).reg,
      .rs = match_and_get(tokens->list[3], REG, 3, line).reg,
      .rn =
          (code.mul_a) ? match_and_get(tokens->list[4], REG, 4, line).reg : 0,
      .accumulate = code.mul_a,
      .set_cond = !SET};

  return construct_mult_binary(&mult_instr);
}

uint32_t parse_sdt(void *ll_node, union instr_code code, symbol_table *st) {
  sdt_t instr = {0};
  node *node = ll_node;
  token_list *tokens = node->value;
  uint32_t line = (node->address / 4) + 1;

  instr.cond = AL;
  instr.load = code.sdt_l;

  uint8_t pos = 1;

  // The first operand is always Rd
  instr.rd = get_and_move(tokens->list[pos], REG, &pos, line).reg;

  // The next token must be '=' or '['
  assert_token(tokens->list[2].type == SEPARATOR, 2, line);
  if (tokens->list[2].data.separator == '=') {
    // '=' is only available for ldr instructions
    assert_token(code.sdt_l == 1, 2, line);
    assert_token(tokens->list[3].type == EXPRESSION, 3, line);

    uint32_t to_load = tokens->list[3].data.exp;

    if (to_load <= 0xff) {
      // we assemble this as a mov instruction and return immediately
      dataproc_t dataproc_instr = {
          .cond = AL,
          .is_immediate = SET,
          .opcode = MOV,
          .rd = instr.rd,
      };

      uint8_t currptr = 3;
      dataproc_instr.op2 = parse_operand2(tokens, &currptr, line);
      return construct_dataproc_binary(&dataproc_instr);

    } else {
      uint32_t *val = calloc(1, sizeof(uint32_t));
      instr.offset = append_via_node(ll_node, val) - (node->address + 8);
      instr.is_shift_R = !SET;
      instr.is_preindexed = SET;
      instr.up_bit = SET;
      instr.rn = PC;
      instr.rd = instr.rd;
      *val = to_load;
      return construct_sdt_binary(&instr);
    }
  }

  cond_move(assert_separator(tokens->list[pos], '[', pos, line), &pos);
  instr.rn = get_and_move(tokens->list[pos], REG, &pos, line).reg;

  // Check post / pre-indexed
  instr.is_preindexed = match_separator(tokens->list[tokens->size - 1], ']');

  // No offset
  if (tokens->size == 5) {
    instr.is_shift_R = !SET;
    instr.offset = 0;
    instr.up_bit = SET;
    return construct_sdt_binary(&instr);
  }

  instr.is_shift_R = cond_move(match_separator(tokens->list[pos], ']'), &pos)
                         ? !SET
                         : instr.is_shift_R;

  if (cond_move(match_separator(tokens->list[pos], '#'), &pos)) {
    instr.up_bit = !cond_move(match_separator(tokens->list[pos], '-'), &pos);
  } else {
    instr.is_shift_R = SET;
    instr.up_bit = SET;
  }

  instr.offset = parse_operand2(tokens, &pos, line);

  if (instr.is_preindexed) {
    assert_separator(tokens->list[pos], ']', pos, line);
  }

  return construct_sdt_binary(&instr);
}

uint32_t parse_branch(void *ll_node, union instr_code code, symbol_table *st) {
  node *node = ll_node;
  token_list *tokens = node->value;
  uint32_t line = (node->address / 4) + 1;

  uint32_t *label_address = (uint32_t *)st_retrieve(
      st, match_and_get(tokens->list[1], LABEL, 1, line).label);

  branch_t branch_instr = {
    .cond = code.branch_cond,
    .offset =
      ((int32_t)*label_address - (int32_t)node->address - 8) >> 2
  };

  return construct_branch_binary(&branch_instr);
}


uint32_t parse_lsl(void *ll_node, union instr_code code, symbol_table *st) {
  char fake_input[511];
  node *old_node = ll_node;
  token_list *tokens = old_node->value;

  sprintf(fake_input, "mov r%d,r%d, lsl #%d"
    , tokens->list[1].data.reg
    , tokens->list[1].data.reg
    , tokens->list[3].data.exp);

  token_list *new_tokens = tokenizer(fake_input);

  /* Stack based assignment of new_node */
  // node new_node;
  // new_node.address = old_node->address;
  // new_node.value = new_tokens;

  /* Hardcore malloc'd version */
  node *new_node = init_node(old_node->address, new_tokens);
  uint32_t result = parse_dataproc(new_node, code, st);

  /* With hardcore freeing */
  free(new_node);

  return result;
}
