#ifndef EMULATE_H
#define EMULATE_H
#include <stdint.h>

typedef struct dataproc_t {
  uint8_t cond : 4;
  uint8_t is_immediate : 1;
  uint8_t opcode : 4;
  uint8_t set_cond : 1;
  uint8_t rn : 4;
  uint8_t rd : 4;
  uint16_t op2 : 12;
} dataproc_t;

typedef struct multiply_t {
  uint8_t cond : 4;
  uint8_t accumulate : 1;
  uint8_t set_cond : 1;
  uint8_t rd : 4;
  uint8_t rn : 4;
  uint8_t rs : 4;
  uint8_t rm : 4;
} multiply_t;

typedef struct sdt_t {
  uint8_t cond : 4;
  uint8_t is_offset : 1;
  uint8_t is_preindexed : 1;
  uint8_t up_bit : 1;
  uint8_t load : 1;
  uint8_t rn : 4;
  uint8_t rd : 4;
  uint16_t offset : 12;
} sdt_t;

typedef struct branch_t {
  uint8_t cond : 4;
  uint32_t offset : 24;
} branch_t;

enum instr_type {
  RAW, DATAPROC, MULTIPLY, SDT, BRANCH
};

union instr_data {
  uint32_t raw_data;
  dataproc_t dataproc;
  multiply_t multiply;
  sdt_t sdt;
  branch_t branch;
};

typedef struct instruction_t {
  enum instr_type tag;
  union instr_data data;
} instruction_t;

typedef struct pipeline_t {
  instruction_t *fetched;
  instruction_t *decoded;
  instruction_t *executed;
} pipeline_t;

typedef struct arm11_state_t {
  uint32_t main_memory[65535];
  uint32_t register_file[16];
  pipeline_t *pipeline;
} machine_state_t;

#endif
