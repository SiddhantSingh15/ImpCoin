#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MEM_SIZE 65536
#define NUM_REGS 17
#define NUM_GENERAL 12
#define WORD_SIZE_IN_BYTES 4

#define PC 15
#define CPSR 16

/* OTHER FLAGS */
#define SET 1
#define NOT_SET 0
#define V_FLAG 28
#define C_FLAG 29
#define Z_FLAG 30
#define N_FLAG 31

/* Different byte sizes to use somewhere */
#define ONE_B 8
#define TWO_B 16
#define THREE_B 24


/* Common register sizes */
#define REG_SIZE 4
#define COND_SIZE 4

/* Common register start positions */
#define COND_POS 28
#define SET_COND_POS 20
#define S_POS 20
#define I_POS 25
#define OFFSET_POS 0
#define RD_POS 12
#define RN_POS 16
#define V_COND 28
#define C_COND 29
#define Z_COND 30
#define N_COND 31

/* DataProc  */
#define OP2_POS 0
#define OP2_SIZE 12
#define OPCODE_POS 21
#define OPCODE_SIZE 4

#define AND 0
#define EOR 1
#define SUB 2
#define RSB 3
#define ADD 4
#define TST 8
#define TEQ 9
#define CMP 10
#define ORR 12
#define MOV 13

/* Multiply  */
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

#endif // DEFINITIONS_H
