#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct machine {
  int *memory;
  int memory_size;
  int *r;
  int *pc;
  int *cpsr;
} machine_t;

void init_machine(machine_t *machine) {
  machine->memory = malloc(16384 * sizeof(int));
  machine.memory_size = 16384;
  machine->r = malloc(16 * sizeof(int));
  machine.pc = machine->r[14];
  machine.cpsr = machine->r[15];
}

void printState(machine_t *machine) {
  // Registers
  printf("Registers:\n");
  for(int i = 0; i < 15; i++) {
    printf("$%d %s:\t\t%d (0x%08x)\n", i,
           (i < 10) ? " " : "",
           machine->r[i], machine->r[i]);
  }

  // Memory
  printf("Non-zero memory:\n");
  for(long i = 0; i < 65536; i += 4) {
    char a = machine->memory[i];
    if(a != 0) printf("Char a: %i\n", a);
    char b = machine->memory[i+1];
    char c = machine->memory[i+2];
    char d = machine->memory[i+3];
    if(a != 0 || b != 0 || c != 0 || d != 0) {
     printf("0x%08lx: 0x%02x%02x%02x%02x\n", i, a, b, c, d);
    }
  }
}

/*
 *  Loads a binary file to [*memory]
 */
void loadfile(char *filename, int *memory, int memory_size) {
  FILE *file = fopen(filename, "r");

  if (file == NULL) {
    printf("File \"%s\" not found\n", filename);
    return;
  }

  fgets(memory, memory_size, file);

  fclose(file);
}

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("Usage: emulate <file>");
    return EXIT_FAILURE;
  }

  machine_t machine;
  init_machine(&machine);

  // Load binary file to memory
  loadfile(argv[1], machine.memory, machine.memory_size);
 
  // Print values of registers & memory
  printState(&machine);

  free((void*) machine.memory);
  free((void*) machine.r);
  return EXIT_SUCCESS;
}
