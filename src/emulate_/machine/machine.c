#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../../emulate.h"
#include "../../instructions.h"

static const uint32_t machine_memory_size = 65536;
static const uint32_t machine_register_count = 16;

void init_machine(machine_t *machine) {
  machine->memory = calloc(machine_memory_size, sizeof(memchunk_t));
  machine->memsize = machine_memory_size;
  machine->registers = calloc(machine_register_count, sizeof(instruction_t));
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
  /* Print register values */
  printf("Registers:\n");
  int loop_read = (machine_register_count < 12) ? machine_register_count : 12;
  for (int i = 0; i <= loop_read; ++i) {
    printf("$%-3i:%11d (0x%08x)\n", i, machine->registers[i], machine->registers[i]);
  }

  /* Print PC and CPSR */
  printf("%s:%11d (0x%08x)\n", "PC  ", *(machine->pc), *(machine->pc));
  printf("%s:%11d (0x%08x)\n", "CPSR", *(machine->cpsr), *(machine->cpsr));

  /* Print non-zero memory */
  printf("Non-zero memory:\n");
  int i;
  for (i = 0; i < (machine_memory_size / sizeof(instruction_t)); ++i) {
    /* Initialise the instruction to the Big Endian format */
    instruction_t instruction = fetch_instruction_pos(machine, i);

    /* Print the instruction / *-byte boundary if non-zero */
    if (instruction != 0) {
      printf("0x%08lx: 0x%08x\n",
              i * sizeof(instruction_t), instruction);
    }
  }
}
