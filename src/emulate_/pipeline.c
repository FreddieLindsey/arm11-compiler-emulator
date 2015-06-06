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

void init_pipeline(pipeline_t *pipeline, machine_t *machine) {
  pipeline->decoded = NULL;
  pipeline->fetched = NULL;
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
    printf("\nCycle No.\t%10i\n", *(machine->pc) / 4);
    printf("Decoded instruction:\n");
    decode(machine);
    fetch(machine);
    printf("Fetched:\t0x%08x\n", *(machine->pipeline->fetched));
    *(machine->pc) += sizeof(instruction_t);
    result = execute(machine);
  } while (result != -1 && *(machine->pc) < 16/* machine->memsize */);
}

/*
 *  Returns -1 iff the instruction is halt.
 *  Returns  0 otherwise.
 */
static int execute(machine_t *machine) {
  /* Execute the decoded instruction in decoded */
  if (machine->pipeline->decoded != NULL) {
    return instruction_execute(machine->pipeline->decoded, machine);
  }
  return 0;
}

static void decode(machine_t *machine) {
  /* Set decoded to be the decoded instruction of fetched */
  if (machine->pipeline->fetched != NULL) {
    printf("Fetched not null, decoding\n");
    machine->pipeline->decoded =
      instruction_decode(machine->pipeline->fetched);
  }
}

static void fetch(machine_t *machine) {
  *(machine->pipeline->fetched) = fetch_instruction(machine);
}
