#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "machine.h"

static const uint32_t machine_memory_size = 65536;
static const uint32_t machine_register_count = 16;

void init_machine(machine_t *machine) {
  machine->memory = calloc(machine_memory_size, sizeof(memchunk_t));
  machine->memsize = machine_memory_size;
  machine->registers = calloc(machine_register_count, sizeof(memchunk_t));
  machine->regcount = machine_register_count;
}

void close_machine(machine_t *machine) {
  free(machine->memory);
  free(machine->registers);
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
  for (int i = 0; i < (machine_memory_size * sizeof(memchunk_t) / sizeof(instruction_t)); ++i) {
    memchunk_t *memoryposition = &machine->memory[(i+1) * sizeof(instruction_t) / sizeof(memchunk_t) - 1];
    instruction_t instruction = *(memoryposition - 3) << 24 | *(memoryposition - 2) << 16 |
                                *(memoryposition - 1) <<  8 | *(memoryposition);

    if (instruction != 0) { 
      printf("Memory Location 0x%08lx:\t0x%08x\n", 
              i * (sizeof(instruction_t) / sizeof(memchunk_t)), instruction);
    }
  }

  /* Signal end of machine to user */
  printf("\nEnd of machine state.\n");
}
