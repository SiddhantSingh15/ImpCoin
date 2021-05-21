#ifndef EMULATE_H
#define EMULATE_H
#include <stdint.h>

typedef struct pipeline_t {
  //instruction_t *fetched;
  //instruction_t *decoded;
  //instruction_t *executed;
} pipeline_t;

typedef struct machine_state_t {
  uint8_t main_memory[65535];
  uint8_t register_file[16];
  pipeline_t *pipeline;
} machine_state_t;

#endif
