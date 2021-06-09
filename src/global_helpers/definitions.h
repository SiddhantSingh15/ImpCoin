#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MEM_SIZE 65536
#define READ_SIZE 63356
#define NUM_REGS 17
#define NUM_GENERAL 12
#define INSTR_SIZE 32
#define WORD_SIZE_IN_BYTES 4
#define TWENTY_FOUR_MSP 23

#define PC 15
#define CPSR 16

/* Macro for creating 32-bit set-masks of size n */
#define MASK(n) (uint32_t) ((1 << n) - 1)
#define TOP_EIGHT_BITMASK 0xFF000000

/* FLAGS */
#define SET 1
#define RESET 0
#define V_FLAG 28
#define C_FLAG 29
#define Z_FLAG 30
#define N_FLAG 31
#define INSTR_FLAG 26
#define DP_FLAG 0x0
#define SDT_FLAG 0x1
#define BRANCH_FLAG 0x2

/* Different byte sizes to use somewhere */
#define ONE_B 8
#define TWO_B 16
#define THREE_B 24


/* Common register sizes */
#define REG_SIZE 4
#define COND_SIZE 4

/* Common register start positions */
#define COND_POS 28
#define S_POS 20
#define I_POS 25
#define OFFSET_POS 0
#define RD_POS 12
#define RN_POS 16

/* DataProc  */
#define OP2_POS 0
#define OP2_SIZE 12
#define OPCODE_POS 21
#define OPCODE_SIZE 4
#define IMM_VALUE_SIZE 8

/* Multiply  */
#define ZERO_POS 22 
#define NINE_POS 4
#define RM_POS 0
#define RS_POS 8
#define MULTIPLY_RN_POS 12
#define MULTIPLY_RD_POS 16
#define A_POS 21

/* SDT  */
#define SDT_OFFSET_SIZE 12
#define L_POS 20
#define U_POS 23
#define P_POS 24

/* Branch */
#define BRANCH_OFFSET 24

/* Condition codes suffixes */
#define EQ 0x0
#define NE 0x1
#define GE 0xA
#define LT 0xB
#define GT 0xC
#define LE 0xD
#define AL 0xE

/* DP OpCodes  */
#define AND 0x0
#define EOR 0x1
#define SUB 0x2
#define RSB 0x3
#define ADD 0x4
#define TST 0x8
#define TEQ 0x9
#define CMP 0xA
#define ORR 0xC
#define MOV 0xD
#define ANDEQ 0xE

/* Shift Operations */
#define SHIFT_MULTIPLIER 2
#define SHIFT_TYPE_POS 5
#define SHIFT_TYPE_SIZE 2
#define SHIFT_VAL 7
#define SHIFT_VAL_SIZE 5
#define LSL 0x0
#define LSR 0x1
#define ASR 0x2
#define ROR 0x3

/* Symbol Table */
#define TABLE_CAPACITY 64

/* Tokenizer */
#define DELIMITERS " ,[]#=+-"
#define BLANK_SPACE " "
#define COMMA_SPACE ", "
#define REGISTER_SYMB 'r'
#define LINE_TERMINATOR '\0'
#define BINARY 2
#define OCTAL 8
#define DENARY 10
#define HEX 16

/* File Write */
#define WRITE_SIZE 1

#endif // DEFINITIONS_H
