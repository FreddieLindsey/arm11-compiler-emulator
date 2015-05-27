#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "machine.h"

void loadfile(FILE *file, machine_t *machine) {
  /* Ensure file is not NULL */
  if (file == NULL) {
    printf("File given to loadfile() is NULL\n");
    exit(EXIT_FAILURE);
  }

  /* Read the file into memory. */
  for (int i = 0; i < (machine->memsize / sizeof(instruction_t)); ++i) {
    /* Read in a byte at a time */
    for (int j = 0; j < sizeof(instruction_t); ++j) {
      fread(&machine->memory[i * sizeof(instruction_t) + j], sizeof(memchunk_t), 1, file);
    }
  }

  /* Close the file after reading */
  fclose(file); 
}
