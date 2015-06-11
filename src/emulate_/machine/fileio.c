#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../../emulate.h"

void loadfile(FILE *file, machine_t *machine) {
  /* Ensure file is not NULL */
  if (file == NULL) {
    printf("File given to loadfile() is NULL\n");
    exit(EXIT_FAILURE);
  }

  /* Read the file into memory. N.B. Retains formatting of data (LE/BE) */
  if (fread(machine->memory, machine->memsize, sizeof(memchunk_t), file) != 0) {
    printf("Not all input read into file. EXITING\n");
    exit(EXIT_FAILURE);
  }

  /* Close the file after reading */
  fclose(file);
}
