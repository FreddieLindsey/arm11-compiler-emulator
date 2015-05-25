#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emulate_utils/machine.h"
#include "emulate_utils/printmachine.h"
#include "emulate_utils/loadfiletomemory.h"
#include "emulate_utils/fetchinstruction.h"
#include "emulate_utils/bytetobinary.h"

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("Usage: emulate <file>");
    return EXIT_FAILURE;
  }

  machine_t machine;
  machine.memory = calloc(65536, sizeof(unsigned char)); 
  machine.r = calloc(14, sizeof(int));
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
