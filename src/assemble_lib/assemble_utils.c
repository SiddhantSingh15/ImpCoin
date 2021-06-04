#include "linked_list.h"
#include "symbol_table_utils.h"
#include "tokenizer.h"
#include "tokens.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "symbol_table_utils.h"

#include "../global_helpers/definitions.h"

void write_file(uint32_t *instr_array, int array_size, char *save_name) {
  FILE *file = fopen(save_name, "wb");

  for (int i = 0; i < array_size; i++) {
    fwrite(&instr_array[i], INSTR_SIZE, 1, file);
  }

  fclose(file);
}

// reads the assembly file and populates the linked list and symbol table
void read_asm(char *filename, linked_list *instructions,
              symbol_table *symbols) {

  FILE *fptr;
  char *buffer = NULL;
  size_t bufsize = 511;
  uint32_t mem_address;

  if ((fptr = fopen(filename, "r")) == NULL) {
    printf("Error opening file.\n");
    exit(EXIT_FAILURE);
  }

  while (getline(&buffer, &bufsize, fptr)) {

    if (ferror(fptr)) {
      perror("Error occurred when reading file\n");
      exit(EXIT_FAILURE);
    }

    // remove trailing newline
    buffer[strcspn(buffer, "\n")] = '\0';

    if (buffer[strlen(buffer) - 1] == ':') {
      // it is a label, add to symbol table
      st_insert(symbols, buffer, &mem_address, sizeof(uint32_t));
      continue;
    }

    // tokenize, and add to the linked list
    token_list *tokens = tokenizer(buffer);
    append_to_linked_list(instructions, tokens, mem_address);

    mem_address += 4;
  }

  free(buffer);
  buffer = NULL;

  fclose(fptr);
}

void placeholder_decipher(void *x, union instr_code y, symbol_table *z) {
  return;
}

void populate_st_instructions(symbol_table *st) {
  uint32_t vsize = sizeof(instr_func_map);
  // DATAPROC insertions

  // AND - OPCODE = 0000
  instr_func_map and = {
    .code.dataproc_opcode = AND,
    .function = &placeholder_decipher
  };
  st_insert(st, "and", &and, vsize);

  // EOR - OPCODE = 0001
  instr_func_map eor = {
    .code.dataproc_opcode = EOR,
    .function = &placeholder_decipher
  };
  st_insert(st, "eor", &eor, vsize);

  // SUB - OPCODE = 0010
  instr_func_map sub = {
    .code.dataproc_opcode = SUB,
    .function = &placeholder_decipher
  };
  st_insert(st, "sub", &sub, vsize);

  // RSB - OPCODE = 0011
  instr_func_map rsb = {
    .code.dataproc_opcode = RSB,
    .function = &placeholder_decipher
  };
  st_insert(st, "rsb", &rsb, vsize);

  // ADD - OPCODE = 0100
  instr_func_map add = {
    .code.dataproc_opcode = ADD,
    .function = &placeholder_decipher
  };
  st_insert(st, "add", &add, vsize);

  // TST - OPCODE = 1000
  instr_func_map tst = {
    .code.dataproc_opcode = TST,
    .function = &placeholder_decipher
  };
  st_insert(st, "tst", &tst, vsize);

  // EOR - TEQ = 1001
  instr_func_map teq = {
    .code.dataproc_opcode = TEQ,
    .function = &placeholder_decipher
  };
  st_insert(st, "teq", &teq, vsize);

  // CMP - OPCODE = 1010
  instr_func_map cmp = {
    .code.dataproc_opcode = CMP,
    .function = &placeholder_decipher
  };
  st_insert(st, "cmp", &cmp, vsize);

  // ORR - OPCODE = 1100
  instr_func_map orr = {
    .code.dataproc_opcode = ORR,
    .function = &placeholder_decipher
  };
  st_insert(st, "orr", &orr, vsize);

  // MOV - OPCODE = 1101
  instr_func_map mov = {
    .code.dataproc_opcode = MOV,
    .function = &placeholder_decipher
  };
  st_insert(st, "mov", &mov, vsize);

  // SPECIAL: LSL - OPCODE = MOV = 1101
  instr_func_map lsl = {
    .code.dataproc_opcode = MOV,
    .function = &placeholder_decipher
  };
  st_insert(st, "lsl", &lsl, vsize);

  // SPECIAL: ANDEQ - OPCODE = AND = 0000
  instr_func_map andeq = {
    .code.dataproc_opcode = AND,
    .function = &placeholder_decipher
  };
  st_insert(st, "andeq", &andeq, vsize);

  // MULTIPLY insertions
  instr_func_map mul = {
    .code.mul_a = !SET,
    .function = &placeholder_decipher
  };
  st_insert(st, "mul", &mul, vsize);

  instr_func_map mla = {
    .code.mul_a = SET,
    .function = &placeholder_decipher
  };
  st_insert(st, "mla", &mla, vsize);

  // SDT insertions
  instr_func_map ldr = {
    .code.sdt_l = SET,
    .function = &placeholder_decipher
  };
  st_insert(st, "ldr", &ldr, vsize);

  instr_func_map str = {
    .code.sdt_l = !SET,
    .function = &placeholder_decipher
  };
  st_insert(st, "str", &str, vsize);

  // BRANCH insertions
  instr_func_map b = {
    .code.branch_cond = AL,
    .function = &placeholder_decipher
  };
  st_insert(st, "b", &b, vsize);

  instr_func_map bal = {
    .code.branch_cond = AL,
    .function = &placeholder_decipher
  };
  st_insert(st, "bal", &bal, vsize); // b and bal do the same thing

  instr_func_map beq = {
    .code.branch_cond = EQ,
    .function = &placeholder_decipher
  };
  st_insert(st, "beq", &beq, vsize);

  instr_func_map bne = {
    .code.branch_cond = NE,
    .function = &placeholder_decipher
  };
  st_insert(st, "bne", &bne, vsize);

  instr_func_map bge = {
    .code.branch_cond = GE,
    .function = &placeholder_decipher
  };
  st_insert(st, "blt", &bge, vsize);

  instr_func_map blt = {
    .code.branch_cond = LT,
    .function = &placeholder_decipher
  };
  st_insert(st, "blt", &blt, vsize);

  instr_func_map bgt = {
    .code.branch_cond = GT,
    .function = &placeholder_decipher
  };
  st_insert(st, "bgt", &bgt, vsize);

  instr_func_map ble = {
    .code.branch_cond = LE,
    .function = &placeholder_decipher
  };
  st_insert(st, "ble", &ble, vsize);
}

int8_t is_shift(char *token) {
  if (!strcmp(token, "lsl")) {
    return LSL;
  } else if (!strcmp(token, "lsr")) {
    return LSR;
  } else if (!strcmp(token, "asr")) {
    return ASR;
  } else if (!strcmp(token, "ror")) {
    return ROR;
  } else {
    return -1;
  }
}

