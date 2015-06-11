#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate.h"
#include "../instructions.h"

void fetch_instruction(machine_t *machine) {
  *(machine->pipeline->fetched) =
    fetch_instruction_pos_format(machine,
                                (*(machine->pc) / sizeof(instruction_t)));
}

/*
 *  Fetches the number of bytes as per the instruction_t alias
 *  Since memory is in big-endian, converts to little endian
 *  for human readability.
 */
instruction_t fetch_instruction_pos_format(machine_t *machine,
  addressable_t mempos) {
  /* Initialise the instruction to 0, get the memory pointer */
  instruction_t instruction = 0;
  memchunk_t *memposptr =
    &machine->memory[(mempos+1) * sizeof(instruction_t) - 1];

  /* Compile the instruction swapping between Little-Endian and Big-Endian */
  int j;
  for (j = 0; j < sizeof(instruction_t); ++j) {
    instruction |=
      *(memposptr - j) << ((sizeof(instruction_t) - j - 1) * CHAR_BIT);
  }
  return instruction;
}

/*
 *  Fetches the number of bytes as per the instruction_t alias
 *  Retains memory formatting
 */
instruction_t fetch_instruction_pos(machine_t *machine, addressable_t mempos) {
  /* Initialise the instruction to 0, get the memory pointer */
  instruction_t instruction = 0;
  memchunk_t *memposptr =
    &machine->memory[mempos * sizeof(instruction_t)];

  /* Compile the instruction */
  int j;
  for (j = 0; j < sizeof(instruction_t); ++j) {
    instruction |=
      *(memposptr + j) << ((sizeof(instruction_t) - j - 1) * CHAR_BIT);
  }
  return instruction;
}
