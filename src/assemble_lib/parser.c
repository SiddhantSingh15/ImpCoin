#include "../global_helpers/definitions.h"
#include "../global_helpers/types.h"
#include "symbol_table.h"
#include "tokens.h"
#include "tokenizer.h"
#include "linked_list.h"
#include "parser.h"
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
  }
  return token_cond;
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
  return tok.type == SEPARATOR && tok.data.separator == separator;
}

union token_data get_and_move(struct token tok, enum token_type expected,
                               uint8_t *pos, uint8_t line) {
    cond_move(assert_token(tok.type == expected, *pos, line), pos);;
    return tok.data;
}

union token_data match_and_get(struct token tok, enum token_type expected,
                               uint8_t pos, uint8_t line) {
    assert_token(tok.type == expected, pos, line);
    return tok.data;
}

uint16_t parse_operand2(token_list *tokens, uint8_t line, uint8_t *pos) {

  // If op2 is an immediate value (EXPR identified)
  if (tokens->list[*pos].type == EXPRESSION) {
    // Set up rotate byte
    // Rotation example
    // 1. 00 00 xx 00 -> rotate = 16 - 2
    // 2. 00 00 0x x0 -> rotate = 16 - 1
    // 3. 00 00 00 xx -> no rotation
    uint16_t rotate_count = ROTATE_COUNT;
    // Rotate only if it is required - If value is less than 0xFF,
    // no rotation is needed
    if (tokens->list[*pos].data.exp > 0xFF) {
      // 0x3 = 11 in binary, this while loop checks to see if the last  any of
      // the 2 bits are set E.G. 1100 is not valid, 0011 is valid, 0110 is valid
      while (!(tokens->list[*pos].data.exp & 0x3)) {
        rotate_count--;
        tokens->list[*pos].data.exp >>= SHIFT_MULTIPLIER;
      }
    }

    rotate_count <<= ONE_B;
    (*pos)++;
    return rotate_count | tokens->list[*pos - 1].data.exp;
  }

  // op2 is not immediate, first token will be a register
  uint8_t rm = get_and_move(tokens->list[*pos], REG, pos, line).reg;

  // No shift present
  if (tokens->size == *pos - 1 || tokens->list[*pos].type != SHIFTNAME) {
    return (uint16_t)rm;
  }

  // Shift present
  uint8_t shift =
      get_and_move(tokens->list[*pos], SHIFTNAME, pos, line).shift_name << 1;

  // Structure of shift
  // 1. <shiftname> <#expression> - 3 tokens
  // 2. <shiftname> <register> - 2 tokens
  if (tokens->list[*pos].type == SEPARATOR) {
    // Case 1: <shiftname> <register> - 3 tokens
    cond_move(assert_separator(tokens->list[*pos], '#', *pos, line), pos);
    shift |= get_and_move(tokens->list[*pos], EXPRESSION, pos, line).exp << 3;
  } else {
    // Case 2: <shiftname> <#expression> - 2 tokens
    shift |= get_and_move(tokens->list[*pos], REG, pos, line).reg << 4;
    shift++;
  }

  uint16_t base = rm;
  base |= shift << SHIFT_OFFSET;
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

    instr.op2 = parse_operand2(tokens, line, &pos);
    return construct_dataproc_binary(&instr);
  }

  if (code.dataproc_opcode == MOV) {
    // Check for MOV
    // 0 - INSTR, 1 - RD, 2  - <#expression>
    instr.rd = get_and_move(tokens->list[pos], REG, &pos, line).reg;
    instr.is_immediate =
        cond_move(match_separator(tokens->list[pos], '#'), &pos);

    instr.op2 = parse_operand2(tokens, line, &pos);
    return construct_dataproc_binary(&instr);
  }

  // Remaining can only be AND, EOR, SUB, RSB, ADD, ORR
  // 0 - INSTR, 1 - RD, 2 - RN, 3 - <#expression>
  instr.rd = get_and_move(tokens->list[pos], REG, &pos, line).reg;
  instr.rn = get_and_move(tokens->list[pos], REG, &pos, line).reg;
  instr.is_immediate = cond_move(match_separator(tokens->list[pos], '#'), &pos);

  instr.op2 = parse_operand2(tokens, line, &pos);
  return construct_dataproc_binary(&instr);
}

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

uint32_t parse_sdt_immediate(node *node, union instr_code code) {
  token_list *tokens = node->value;
  uint32_t line = (node->address / 4) + 1;

  uint8_t pos = 1;
  uint8_t rd = get_and_move(tokens->list[pos], REG, &pos, line).reg;

  // This function only takes immediate sdt instructions.
  assert_separator(tokens->list[pos], '=', pos, line);
  // This is only available for ldr instructions.
  cond_move(assert_token(code.sdt_l == SET, pos, line), &pos);

  uint32_t to_load =
      match_and_get(tokens->list[pos], EXPRESSION, pos, line).exp;

  if (to_load <= 0xff) {
    // Since the value to load fits inside a mov instruction, we assemble it as
    // such, and return immediately.
    dataproc_t dataproc_instr = {
        .cond = AL,
        .is_immediate = SET,
        .opcode = MOV,
        .rd = rd,
    };

    dataproc_instr.op2 = parse_operand2(tokens, line, &pos);
    return construct_dataproc_binary(&dataproc_instr);
  }

  token_list *val = calloc(1, sizeof(token_list));
  val->list[0].type = EXPRESSION;
  val->list[0].data.exp = to_load;

  sdt_t instr = {
      .cond = AL,
      .load = code.sdt_l,
      .is_preindexed = SET,
      .up_bit = SET,
      .rn = PC,
      .rd = rd,
      .offset = append_via_node(node, val) - (node->address + 8),
  };
  return construct_sdt_binary(&instr);
}

uint32_t parse_sdt(void *ll_node, union instr_code code, symbol_table *st) {
  sdt_t instr = {0};
  node *node = ll_node;
  token_list *tokens = node->value;

  uint32_t line = (node->address / WORD_SIZE_IN_BYTES) + 1;
  uint8_t pos = 1;

  instr.cond = AL;
  instr.load = code.sdt_l;

  // The first operand is always Rd
  instr.rd = get_and_move(tokens->list[pos], REG, &pos, line).reg;

  if (match_separator(tokens->list[pos], '=')) {
    // Immediate SDT is only available for ldr instructions
    assert_token(code.sdt_l == SET, pos, line);
    return parse_sdt_immediate(node, code);
  }

  // Not an immediate instruction, we take the register.
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

  instr.offset = parse_operand2(tokens, line, &pos);

  if (instr.is_preindexed) {
    assert_separator(tokens->list[pos], ']', pos, line);
  }

  return construct_sdt_binary(&instr);
}

uint32_t parse_branch(void *ll_node, union instr_code code, symbol_table *st) {
  node *node = ll_node;
  token_list *tokens = node->value;
  uint32_t line = (node->address / WORD_SIZE_IN_BYTES) + 1;

  uint32_t *label_address = (uint32_t *)st_retrieve(
      st, match_and_get(tokens->list[1], LABEL, 1, line).label);

  branch_t branch_instr = {
    .cond = code.branch_cond,
    .offset =
      ((int32_t)*label_address - (int32_t)node->address - PIPELINE_OFFSET)
      >> BRANCH_SHIFT
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
  free_token_list(new_tokens);
  free(new_tokens);
  free(new_node);

  return result;
}
