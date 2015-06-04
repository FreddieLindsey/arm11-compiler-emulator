#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate.h"
#include "../instructions.h"

static const int MAX_INSTR_SIZE=10

char** instruction_decode(instruction_t instruction) {
  char** result = calloc(sizeof(instruction_t) * CHAR_BIT, MAX_INSTR_SIZE);
  if (result[0] != NULL) {
    printf("Result currently:\t");
    int i = 0;
    while (result[i] != NULL) {
    printf("%s\t", result[i]);
      ++i;
    }
    printf("\n");
  }
  return result;
}
