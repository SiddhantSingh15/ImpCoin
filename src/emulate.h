#ifndef EMULATE_H
#define EMULATE_H
#include <stdint.h>


typedef struct DataProc {
  uint8_t cond : 4;
  uint8_t is_immediate : 1;
  uint8_t opcode : 4;
  uint8_t set_cond : 1;
  uint8_t rn : 4;
  uint8_t rd : 4;
  uint16_t op2 : 12;
} DataProc;

typedef struct Multiply {
  uint8_t cond : 4;
  uint8_t accumulate : 1;
  uint8_t set_cond : 1;
  uint8_t rd : 4;
  uint8_t rn : 4;
  uint8_t rs : 4;
  uint8_t rm : 4;
} Multiply;

typedef struct SDT {
  uint8_t cond : 4;
  uint8_t is_offset : 1;
  uint8_t is_preindexed : 1;
  uint8_t up_bit : 1;
  uint8_t load : 1;
  uint8_t rn : 4;
  uint8_t rd : 4;
  uint16_t offset : 12;
} SDT;

typedef struct Branch {
  uint8_t cond : 4;
  uint32_t offset : 24;
} Branch;

typedef struct instruction_t {
  enum instr_type {Raw, DataProc, Multiply, SDT, Branch};
  union instr_data {
    uint32_t raw_data;
    struct DataProc dataproc;
    struct Multiply multiply;
    struct SDT sdt;
    struct Branch branch;
  };
} instruction_t;

typedef struct pipeline_t {
  instruction_t *fetched;
  instruction_t *decoded;
  instruction_t *executed;
} pipeline_t;

typedef struct machine_state_t {
  uint32_t main_memory[65535];
  uint32_t register_file[16];
  pipeline_t *pipeline;
} machine_state_t;

#endif
