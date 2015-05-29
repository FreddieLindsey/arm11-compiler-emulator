#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../emulate.h"
#include "../instructions.h"

int instruction_execute(char** instruction_decoded, machine_t *machine) {
  if (instruction_decoded != NULL) {
    if (strcmp(instruction_decoded[0], "halt") == 0) return -1;
  }
  return 0;
}
