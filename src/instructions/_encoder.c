#include <stdio.h>
#include <stdlib.h>
#include "../instructions.h"

/*
 *  Encodes a instruction from a decoded instruction struct to its binary
 *  equivalent
 */
instruction_t instruction_encode(decoded_instruction_t *decoded) {
  switch(decoded->kind) {
    case DATA_PROCESS:
      return dataprocess_encode(decoded);
    case MULTIPLY:
      return multiply_encode(decoded);
    case SINGLE_DATA_TRANSFER:
      return singledatatransfer_encode(decoded);
    case BRANCH:
      return branch_encode(decoded);
    case ANDEQ:
      return 0;
    default:
      return -1;
  }
}
