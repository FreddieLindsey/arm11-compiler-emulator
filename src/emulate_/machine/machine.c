#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../../emulate.h"
#include "../../instructions.h"

static const uint32_t machine_memory_size = 65536;
static const uint32_t machine_register_count = 16;
/* static instruction_t big_to_little_end(instruction_t); */

void init_machine(machine_t *machine) {
  machine->memory = calloc(machine_memory_size, sizeof(memchunk_t));
  machine->memsize = machine_memory_size;
  machine->registers = calloc(machine_register_count, sizeof(memchunk_t));
  machine->regcount = machine_register_count;
  machine->pc = machine->registers + (machine->regcount - 2);
  machine->cpsr = machine->registers + (machine->regcount - 1);
  machine->pipeline = calloc(sizeof(pipeline_t), 1);
}

void close_machine(machine_t *machine) {
  free(machine->memory);
  free(machine->registers);
  free(machine->pipeline);
}

void print_machine(machine_t *machine) {
  /* Signal start of machine to user */
  printf("\nPrinting machine state:\n");

  /* Print register values */
  printf("\nRegister values:\n");
  for (int i = 0; i < machine_register_count; ++i) {
    printf("Register No. %2i:\t0x%08x\n", i + 1,
           machine->registers[i]);
  }

  /* Print non-zero memory */
  printf("\nNon-zero memory:\n");
  int i;
  for (i = 0; i < (machine_memory_size / sizeof(instruction_t)); ++i) {
    /* Initialise the instruction to 0 */
    instruction_t instruction = fetch_instruction_pos_be(machine, i);

    /* Print the instruction / *-byte boundary if non-zero */
    if (instruction != 0) { 
      printf("Memory Location 0x%08lx:\t0x%08x\n", 
              i * sizeof(instruction_t), instruction);
    }
  }

  /* Signal end of machine to user */
  printf("\nEnd of machine state.\n");
}

/* TODO
static void big_to_little_end(instruction_t *instruction) {
  int i = (int) (sizeof(instruction_t) / (2 * sizeof(memchunk_t)));
  while (i > 0) {
    instruction_t outside_mask, inside_mask;
    outside_mask = 0;
    memchunk_t temp = instruction << (i) & outside_mask;
  }
}*/
