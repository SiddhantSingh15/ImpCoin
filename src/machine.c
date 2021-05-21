#include <stdint.h>

// For the core structs relating to the emulator

typedef struct pipeline_t {
} pipeline_t;

typedef struct machine_state_t {
  uint8_t main_memory[65535];
  uint8_t register_file[16];
  pipeline_t *pipeline;
} machine_state_t;
