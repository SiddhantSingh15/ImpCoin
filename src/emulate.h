#ifndef EMULATE_H
#define EMULATE_H
#include <stdint.h>

typedef struct dataproc_t {
  uint16_t op2 : 12;
  uint8_t cond : 4;
  uint8_t rn : 4;
  uint8_t opcode : 4;
  uint8_t rd : 4;
  uint8_t is_immediate : 1;
  uint8_t set_cond : 1;
} dataproc_t;

typedef struct multiply_t {
  uint8_t cond : 4;
  uint8_t rd : 4;
  uint8_t rn : 4;
  uint8_t rs : 4;
  uint8_t rm : 4;
  uint8_t accumulate : 1;
  uint8_t set_cond : 1;
} multiply_t;

typedef struct sdt_t {
  uint16_t offset : 12;
  uint8_t cond : 4;
  uint8_t rn : 4;
  uint8_t rd : 4;
  uint8_t is_shift_R : 1;
  uint8_t is_preindexed : 1;
  uint8_t up_bit : 1;
  uint8_t load : 1;
} sdt_t;

typedef struct branch_t {
  uint32_t offset : 24;
  uint8_t cond : 4;
} branch_t;

enum instr_type { RAW, DATAPROC, MULTIPLY, SDT, BRANCH, HALT };

/*
 * This contains raw data, one of the 4 decoded structs.
 *
 * We store the instruction as raw data in the fetch stage,
 * and replace it with the decoded struct in the decode stage.
 *
 */
union instr_data {
  uint32_t raw_data;
  dataproc_t dataproc;
  multiply_t multiply;
  sdt_t sdt;
  branch_t branch;
};

/*
 * The instruction type contains a union type for instructions, and
 * an enum (instr_type) to tag which of the union types is currently
 * being held.
 *
 */
typedef struct instruction_t {
  union instr_data data;
  enum instr_type tag;
} instruction_t;

typedef struct pipeline_t {
  instruction_t *fetched;
  instruction_t *decoded;
  instruction_t *executed;
} pipeline_t;

typedef struct arm11_state_t {
  uint8_t main_memory[65535];
  uint32_t register_file[16];
  pipeline_t *pipeline;
} arm11_state_t;

#endif
