#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../emulate.h"
#include "../instructions.h"

void init_pipeline(pipeline_t *pipeline) {
  pipeline->decoded = NULL;
  pipeline->fetched = 0;
}

void close_pipeline(pipeline_t *pipeline) {
  /* Clean decoded */
  if (pipeline->decoded != NULL) {
    free(pipeline->decoded);
  }
}

void run_pipeline(machine_t *machine) {
  execute(machine);
  decode(machine);
  fetch(machine);
}

void execute(machine_t *machine) {

}

void decode(machine_t *machine) {
  machine->pipeline->decoded = 
    instruction_decode(machine->pipeline->fetched);
}

void fetch(machine_t *machine) {
  machine->pipeline->fetched = 
    machine->memory[machine->registers[machine->regcount - 2]];
}
