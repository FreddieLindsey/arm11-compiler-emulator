#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate.h"
#include "../instructions.h"

char** instruction_decode(instruction_t instruction) {
  char** result = calloc(sizeof(instruction_t) * CHAR_BIT, 
                         sizeof(instruction_t) * CHAR_BIT);
  if (result[0] != NULL) {
    printf("Result currently:\t");
    int i = 0;
    while (result[i] != NULL) {
    printf("%s\t", result[i]);
      ++i;
    }
    printf("\n");
  }
  printf("Value of instruction:\t0x%08x\n", instruction);
  return result;
}
