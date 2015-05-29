#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../emulate.h"
#include "../instructions.h"

/* 
 *  Parts of the pipeline should only be executed
 *  by the pipeline itself
 */
static void execute(machine_t *machine);
static void decode(machine_t *machine);
static void fetch(machine_t *machine);

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

static void execute(machine_t *machine) {

}

static void decode(machine_t *machine) {
  machine->pipeline->decoded = 
    instruction_decode(machine->pipeline->fetched);
}

static void fetch(machine_t *machine) {
  /* Set fetched to be the next  */
  machine->pipeline->fetched = 
    machine->memory[machine->registers[machine->regcount - 2]];
}
