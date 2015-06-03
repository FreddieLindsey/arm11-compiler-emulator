#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate.h"
#include "../instructions.h"

decoded_instruction_t* instruction_decode(instruction_t instruction) {
  int is_branch   = (instruction & 0x0f000000) == 0x0a000000;
  int is_sdt      = (instruction & 0x0c000000) == 0x04000000; 
  int is_mult     = (instruction & 0x0e000000) == 0 && 
                    (instruction & 0x000000f0) == 0x00000090;
  int is_datproc  = 1; /* Whilst this is the last instruction, no implementation necessary */
  if (is_branch) {
    return branch_decode(instruction);
  } else if (is_sdt) {
    return singledatatransfer_decode(instruction);
  } else if (is_mult) {
    return multiply_decode(instruction);
  } else if (is_datproc) {
    return dataprocess_decode(instruction);
  }
  return NULL;
}
