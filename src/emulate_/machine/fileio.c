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
  for (int i = 0; i < machine->memsize; ++i) {
    machine->memory[i] = fgetc(file);
    i++;
  }

  /* Close the file after reading */
  fclose(file); 
}