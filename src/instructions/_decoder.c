#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate_/machine/machine.h"

char** instruction_decode(instruction_t instruction) {
  char** result = calloc(sizeof(instruction_t) * CHAR_BIT, 
                         sizeof(instruction_t) * CHAR_BIT);
  
  return result;
}