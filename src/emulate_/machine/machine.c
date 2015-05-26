#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "machine.h"

static const uint32_t machine_memory_size = 65536;
static const int machine_register_count = 16;

void init_machine(machine_t *machine) {
  machine->memory = calloc(machine_memory_size, sizeof(char));
  machine->memsize = machine_memory_size;
  machine->registers = calloc(machine_register_count, sizeof(int));
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
  for (int i = 0; i < (machine_memory_size / sizeof(memchunk_t)); ++i) {
    if (machine->memory[i] != 0) { 
      printf("Memory Location 0x%08x:\t0x%08x\n", 
              i * 4, machine->memory[i]);
    }
  }

  /* Signal end of machine to user */
  printf("\nEnd of machine state.\n");
}
