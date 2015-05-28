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
  fread(machine->memory, machine->memsize, sizeof(memchunk_t), file);

  /* Close the file after reading */
  fclose(file); 
}
