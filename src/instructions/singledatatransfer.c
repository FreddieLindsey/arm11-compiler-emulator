#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate.h"
#include "../instructions.h"

instruction_t singledatatransfer_encode(decoded_instruction_t *decoded) {
  return 0;
}

decoded_instruction_t* singledatatransfer_decode(instruction_t *instruction) {
  decoded_instruction_t *decoded_instruction =
    calloc(sizeof(decoded_instruction_t), 1);
  decoded_instruction->kind       = SINGLE_DATA_TRANSFER;
  decoded_instruction->cond       = (*instruction & 0xf0000000) >> 28;
  decoded_instruction->accumulate = (*instruction & 0x00200000) != 0;
  decoded_instruction->set        = (*instruction & 0x00100000) != 0;
  decoded_instruction->regd       = (*instruction & 0x000f0000) >> 16;
  decoded_instruction->regn       = (*instruction & 0x0000f000) >> 12;
  decoded_instruction->regs       = (*instruction & 0x00000f00) >> 8;
  decoded_instruction->regm       = (*instruction & 0x0000000f);
  return decoded_instruction;
}

int singledatatransfer_execute(decoded_instruction_t* decoded,
                                machine_t* machine) {
  return 0;
}
