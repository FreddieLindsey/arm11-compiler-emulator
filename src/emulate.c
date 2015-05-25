#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct machine {
  char *memory;
  int *r; // General purpose registers
  int pc; // Other registers
  int cpsr; 
};

void loadfile(char *filename, char *memory);
void printState(struct machine *machine);
char* byteToBinary(int x);

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("Usage: emulate <file>");
    return EXIT_FAILURE;
  }

  // Init machine
  struct machine machine;
  machine.memory = calloc(65536, 1);// set 2^16 1 byte elements to 0
  machine.r = calloc(12, sizeof(int));
  machine.pc = 0;
  machine.cpsr = 0;

  // Load binary file to memory
  loadfile(argv[1], machine.memory);
 
  // Print values of registers & memory
  printState(&machine);

  free((void*) machine.memory);
  free((void*) machine.r);
  return EXIT_SUCCESS;
}

/*
 *  Loads a binary file to [*memory]
 */
void loadfile(char *filename, char *memory) {
  FILE *file = fopen(filename, "r");

  if(file == NULL) {
    printf("File \"%s\" not found\n", filename);
    return;
  }
  
  int x, i = 0;
  while((x = fgetc(file)) != EOF) {
    memory[i] = x;
    i++;
    // print memory as its loading in (debug)
    //printf("%s ", byteToBinary(x));
    //if(i % 4 == 0) printf("\n");
  }

  fclose(file);
}

/*
 *  Prints current values of registers
 */
void printState(struct machine *machine) {
  // Registers
  printf("Registers:\n");
  for(int i = 0; i < 13; i++) {
    printf("$%d %s:\t\t%d (0x%08x)\n", 
        i, (i < 10) ? " " : "", (*machine).r[i], (*machine).r[i]);
  }
  printf("PC  :\t\t%d (0x%08x)\n", (*machine).pc, (*machine).pc);
  printf("CPSR:\t\t%d (0x%08x)\n", (*machine).cpsr, (*machine).cpsr);

  // Memory
  printf("Non-zero memory:\n");
  for(long i = 0; i < 65536; i += 4) {
    char a = (*machine).memory[i];
    char b = (*machine).memory[i+1];
    char c = (*machine).memory[i+2];
    char d = (*machine).memory[i+3];
    if(a != 0 || b != 0 || c != 0 || d != 0) {
     printf("0x%08lx: 0x%02x%02x%02x%02x\n", i, a, b, c, d);
    }
  }
}


/*
 *  Converts a byte to a binary string
 *  Helper function to print out a byte for debugging
 */
char* byteToBinary(int x) {
  static char buffer[9];
  buffer[0] = '\0';

  for(int i = 128; i > 0; i >>= 1) {
    strcat(buffer, ((x & i) == i) ? "1" : "0");
  }

  return buffer;
}
