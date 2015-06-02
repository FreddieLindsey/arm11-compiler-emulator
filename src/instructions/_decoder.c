#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate.h"
#include "../instructions.h"

char** instruction_decode(instruction_t instruction) {
  char** result = calloc(sizeof(instruction_t) * CHAR_BIT,
                         sizeof(instruction_t) * CHAR_BIT);
  
  return result;
}

static void determine(instruction_t instruction, char** result) {
  int is_branch   = (instruction & 0x0f000000) == 0x0a000000;
  int is_sdt      = (instruction & 0x0c000000) == 0x04000000; 
  int is_mult     = (instruction & 0x0e000000) == 0 && 
                    (instruction & 0x000000f0) == 0x00000090;
  int is_datproc  = 1; /* Whilst this is the last instruction, no implementation necessary */
  if (is_branch) {
    branch_decode(instruction, char** result);
  } else if (is_sdt) {
    singledatatransfer_decode(instruction, char** result);
  } else if (is_mult) {
    multiply_decode(instruction, char** result);
  } else {
    dataprocess_decode(instruction, char** result);
  }
}
