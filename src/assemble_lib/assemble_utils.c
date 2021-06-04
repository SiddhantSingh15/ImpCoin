#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "symbol_table_utils.h"

#include "../global_helpers/definitions.h"

void write_file(uint32_t *instr_array, int array_size, char *save_name) {
  FILE *file = fopen(save_name, "wb");

  for (int i = 0; i < array_size; i++) {
    fwrite(&instr_array[i], INSTR_SIZE, 1, file);
  }

  fclose(file);
}

void placeholder_decipher(void *x, union instr_code y, void *z) {
  return;
}

void populate_st_instructions(symbol_table *st) {
  // DATAPROC insertions
  // AND - OPCODE = 0000
  instr_func_map *and = calloc(1, sizeof(instr_func_map));
  and->code.dataproc_opcode = AND;
  and->function = &placeholder_decipher;
  st_insert(st, "and", and);
  // EOR - OPCODE = 0001
  instr_func_map *eor = calloc(1, sizeof(instr_func_map));
  eor->code.dataproc_opcode = EOR;
  eor->function = &placeholder_decipher;
  st_insert(st, "eor", eor);
  // SUB - OPCODE = 0010
  instr_func_map *sub = calloc(1, sizeof(instr_func_map));
  sub->code.dataproc_opcode = SUB;
  sub->function = &placeholder_decipher;
  st_insert(st, "sub", sub);
  // RSB - OPCODE = 0011
  instr_func_map *rsb = calloc(1, sizeof(instr_func_map));
  rsb->code.dataproc_opcode = RSB;
  rsb->function = &placeholder_decipher;
  st_insert(st, "rsb", rsb);
  // ADD - OPCODE = 0100
  instr_func_map *add = calloc(1, sizeof(instr_func_map));
  add->code.dataproc_opcode = ADD;
  add->function = &placeholder_decipher;
  st_insert(st, "add", add);
  // TST - OPCODE = 1000
  instr_func_map *tst = calloc(1, sizeof(instr_func_map));
  tst->code.dataproc_opcode = TST;
  tst->function = &placeholder_decipher;
  st_insert(st, "tst", tst);
  // EOR - TEQ = 1001
  instr_func_map *teq = calloc(1, sizeof(instr_func_map));
  teq->code.dataproc_opcode = TEQ;
  teq->function = &placeholder_decipher;
  st_insert(st, "add", teq);
  // CMP - OPCODE = 1010
  instr_func_map *cmp = calloc(1, sizeof(instr_func_map));
  cmp->code.dataproc_opcode = CMP;
  cmp->function = &placeholder_decipher;
  st_insert(st, "cmp", cmp);
  // ORR - OPCODE = 1100
  instr_func_map *orr = calloc(1, sizeof(instr_func_map));
  orr->code.dataproc_opcode = ORR;
  orr->function = &placeholder_decipher;
  st_insert(st, "orr", orr);
  // MOV - OPCODE = 1101
  instr_func_map *mov = calloc(1, sizeof(instr_func_map));
  mov->code.dataproc_opcode = MOV;
  mov->function = &placeholder_decipher;
  st_insert(st, "mov", mov);
  // SPECIAL: LSL - OPCODE = MOV = 1101
  instr_func_map *lsl = calloc(1, sizeof(instr_func_map));
  lsl->code.dataproc_opcode = MOV;
  lsl->function = &placeholder_decipher;
  st_insert(st, "lsl", lsl);
  // SPECIAL: ANDEQ - OPCODE = AND = 0000
  instr_func_map *andeq = calloc(1, sizeof(instr_func_map));
  andeq->code.dataproc_opcode = AND;
  andeq->function = &placeholder_decipher;
  st_insert(st, "andeq", andeq);

  // MULTIPLY insertions
  instr_func_map *mul = calloc(1, sizeof(instr_func_map));
  mul->function = &placeholder_decipher;
  mul->code.mul_a = !SET;
  st_insert(st, "mul", mul);
  instr_func_map *mla = calloc(1, sizeof(instr_func_map));
  mla->function = &placeholder_decipher;
  mla->code.mul_a = SET;
  st_insert(st, "mla", mla);

  // SDT insertions
  instr_func_map *ldr = calloc(1, sizeof(instr_func_map));
  ldr->function = &placeholder_decipher;
  ldr->code.sdt_l = SET;
  st_insert(st, "ldr", ldr);
  instr_func_map *str = calloc(1, sizeof(instr_func_map));
  str->function = &placeholder_decipher;
  str->code.sdt_l = !SET;
  st_insert(st, "str", str);

  // BRANCH insertions
  instr_func_map *b = calloc(1, sizeof(instr_func_map));
  b->function = &placeholder_decipher;
  b->code.branch_cond = AL;
  st_insert(st, "b", b);
  instr_func_map *bal = calloc(1, sizeof(instr_func_map));
  bal->function = &placeholder_decipher;
  bal->code.branch_cond = AL;
  st_insert(st, "bal", bal); // b and bal do the same thing
  instr_func_map *beq = calloc(1, sizeof(instr_func_map));
  beq->function = &placeholder_decipher;
  beq->code.branch_cond = EQ;
  st_insert(st, "beq", beq);
  instr_func_map *bne = calloc(1, sizeof(instr_func_map));
  bne->function = &placeholder_decipher;
  bne->code.branch_cond = NE;
  st_insert(st, "bne", bne);
  instr_func_map *bge = calloc(1, sizeof(instr_func_map));
  bge->function = &placeholder_decipher;
  bge->code.branch_cond = GE;
  st_insert(st, "blt", bge);
  instr_func_map *blt = calloc(1, sizeof(instr_func_map));
  blt->function = &placeholder_decipher;
  blt->code.branch_cond = LT;
  st_insert(st, "blt", blt);
  instr_func_map *bgt = calloc(1, sizeof(instr_func_map));
  bgt->function = &placeholder_decipher;
  bgt->code.branch_cond = GT;
  st_insert(st, "bgt", bgt);
  instr_func_map *ble = calloc(1, sizeof(instr_func_map));
  ble->function = &placeholder_decipher;
  ble->code.branch_cond = LE;
  st_insert(st, "ble", ble);
}