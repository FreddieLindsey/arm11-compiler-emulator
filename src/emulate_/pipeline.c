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
  pipeline->fetched = NULL;
}

void close_pipeline(pipeline_t *pipeline) {
  /* Clean decoded */
  if (pipeline->decoded != NULL) {
    free(pipeline->decoded);
  }
  /* Clean fetched */
  if (pipeline->fetched != NULL) {
    free(pipeline->fetched);
  }
}

void run_pipeline(machine_t *machine) {
  do {
    decode(machine);
    fetch(machine);
    *(machine->pc) += sizeof(instruction_t);
  } while (execute(machine) != -1 && *(machine->pc) < machine->memsize);
}

/*
 *  Returns -1 iff the instruction is halt.
 *  Returns  0 otherwise.
 */
static int execute(machine_t *machine) {
  /* Execute the decoded instruction in decoded */
  if (machine->pipeline->decoded != NULL) {
    return instruction_execute(machine);
  }
  return 0;
}

static void decode(machine_t *machine) {
  if (machine->pipeline->fetched != NULL) {
    if (machine->pipeline->decoded != NULL) {
      free(machine->pipeline->decoded);
    }
    machine->pipeline->decoded = calloc(sizeof(decoded_instruction_t), 1);

    /* Set decoded to be the decoded instruction of fetched */
    instruction_decode(machine->pipeline);
  }
}

static void fetch(machine_t *machine) {
  if (machine->pipeline->fetched != NULL) {
    free(machine->pipeline->fetched);
  }
  machine->pipeline->fetched = calloc(sizeof(instruction_t), 1);

  fetch_instruction(machine);
}
