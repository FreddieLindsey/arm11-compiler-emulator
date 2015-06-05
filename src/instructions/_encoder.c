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
      return encode_data_process(decoded);
    case MULTIPLY:
      return encode_multiply(decoded);
    case SINGLE_DATA_TRANSFER:
      return encode_single_data_transfer(decoded);
    case BRANCH:
      return encode_branch(decoded);
    case ANDEQ:
      return 0;
    default:
      return -1;
  }
}
