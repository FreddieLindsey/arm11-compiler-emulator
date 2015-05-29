#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../emulate.h"
#include "../instructions.h"

/* 
 *  Parts of the pipeline should only be executed
 *  by the pipeline itself
 */
static int execute(machine_t *machine);
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
  int result;
  do {
  decode(machine);
  fetch(machine);
  ++(machine->pc);
  result = execute(machine);
  } while (result != -1);
}

/*
 *  Returns -1 iff the instruction is halt.
 *  Returns  0 otherwise.
 */
static int execute(machine_t *machine) {
  if (machine->pipeline->decoded != NULL) {
    return instruction_execute(machine->pipeline->decoded, machine);
  }
  return 0;
}

static void decode(machine_t *machine) {
  machine->pipeline->decoded = 
    instruction_decode(machine->pipeline->fetched);
}

static void fetch(machine_t *machine) {
  /* Set fetched to be the next instruction */
  machine->pipeline->fetched = 
    machine->memory[machine->registers[machine->regcount - 2]];
}
