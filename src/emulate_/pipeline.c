#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pipeline.h"
#include "../instructions/_decoder.h"

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

void run_pipeline(pipeline_t *pipeline, machine_t *machine) {
  execute(pipeline, machine);
  decode(pipeline, machine);
  fetch(pipeline, machine);
}

void execute(pipeline_t *pipeline, machine_t *machine) {

}

void decode(pipeline_t *pipeline, machine_t *machine) {
  pipeline->decoded = instruction_decode(pipeline->fetched);
}

void fetch(pipeline_t *pipeline, machine_t *machine) {
  pipeline->fetched = 
    machine->memory[machine->registers[machine->regcount - 2]];
}
