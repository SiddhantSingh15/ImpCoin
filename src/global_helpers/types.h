#ifndef EMULATE_H
#define EMULATE_H
#include <stdint.h>
#include "definitions.h"

/**
 * @brief Data processing instruction type
 */
typedef struct dataproc_t {
  uint16_t op2 : OP2_SIZE; /** Operand2 */
  uint8_t cond : COND_SIZE; /** Condition field */
  uint8_t rn : REG_SIZE; /** First operand register */
  uint8_t opcode : REG_SIZE; /** Operation code */
  uint8_t rd : REG_SIZE; /** Destination register */
  uint8_t is_immediate : SET; /** Immediate operand flag */
  uint8_t set_cond : SET; /** Set condition codes flag */
} dataproc_t;

/**
 * @brief Multiply instructions type
 */
typedef struct multiply_t {
  uint8_t cond : COND_SIZE; /** Condition field */
  uint8_t rd : REG_SIZE; /** Destination register */
  uint8_t rn : REG_SIZE; /** Operand Register */
  uint8_t rs : REG_SIZE; /** Operand Register */
  uint8_t rm : REG_SIZE; /** Operand Register */
  uint8_t accumulate : SET; /** Accumulate flag */
  uint8_t set_cond : SET; /** Set condition codes flag */
} multiply_t;

/**
 * @brief Single data transfter instruction type
 */
typedef struct sdt_t {
  uint16_t offset : SDT_OFFSET_SIZE; /** Unsigned 12 bit offset or register */
  uint8_t cond : COND_SIZE; /** Condition field */
  uint8_t rn : REG_SIZE; /** Base register */
  uint8_t rd : REG_SIZE; /** Source/ Destination register */
  uint8_t is_shift_R : SET; /** Immediate offset */
  uint8_t is_preindexed : SET; /** Pre/ Post indexing flag */
  uint8_t up_bit : SET; /** Up bit flag */
  uint8_t load : SET; /** Load/ Store flag */
} sdt_t;

/**
 * @brief Branch instruction type
 */
typedef struct branch_t {
  uint32_t offset : THREE_B; /** Signed 24 bit offset */
  uint8_t cond : COND_SIZE; /** Condition field */
} branch_t;

enum instr_type { RAW, DATAPROC, MULTIPLY, SDT, BRANCH, HALT };

/**
 * @brief
 * This contains raw data, one of the 4 decoded structs.
 *
 * We store the instruction as raw data in the fetch stage,
 * and replace it with the decoded struct in the decode stage.
 */
union instr_data {
  uint32_t raw_data; /** Undecoded binary data */
  dataproc_t dataproc; /** Data Processing instruction */
  multiply_t multiply; /** Multiply instruction */
  sdt_t sdt; /** Single Data Transfer instruction */
  branch_t branch; /** Branch instruction */
};

/**
 * @brief
 * The instruction type contains a union type for instructions, and
 * an enum (instr_type) to tag which of the union types is currently
 * being held.
 */
typedef struct instruction_t {
  union instr_data data; /** Contents of the instruction */
  enum instr_type tag; /** Type of the instruction */
} instruction_t;

/**
 * @brief The pipeline contains the fetched, decoded and executed instructions.
 */
typedef struct pipeline_t {
  instruction_t *fetched; /** Fetched instruction */
  instruction_t *decoded; /** Decoded instruction */
  instruction_t *executed; /** Executed instruction */
} pipeline_t;

/**
 * @brief The whole ARM11 machine.
 */
typedef struct arm11_state_t {
  uint8_t main_memory[MEM_SIZE]; /** The main memory */
  uint32_t register_file[NUM_REGS]; /** Array containing all registers */
  pipeline_t *pipeline; /** Current pipeline of the system */
} arm11_state_t;

/**
 * @brief A hashtable that will contain the label-address pairs
 */
typedef struct symbol_table {
  uint32_t capacity;
  uint32_t count;
  struct symbol_table_kvp **kvps;
} symbol_table;

/**
 * @brief The key-value pair structure for the symbol table 
 */
typedef struct symbol_table_kvp {
  char *key;
  uint32_t value;
} symbol_table_kvp;

#endif
