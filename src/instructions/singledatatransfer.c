#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate.h"
#include "../instructions.h"

instruction_t singledatatransfer_encode(decoded_instruction_t *decoded) {
  const int OFFSET_COND = 0x1C;
  const int OFFSET_1 = 0x1A;
  const int OFFSET_I = 0x19;
  const int OFFSET_P = 0x18;
  const int OFFSET_U = 0x17;
  const int OFFSET_L = 0x14;
  const int OFFSET_RN = 0x10;
  const int OFFSET_RD = 0xC;
  const int OFFSET_OFFSET = 0x0;
  const int COND = 14;
  return (COND << OFFSET_COND) | (0x1 << OFFSET_1) | 
      (decoded->immediate << OFFSET_I) | (decoded->prepost << OFFSET_P) |
      (decoded->up << OFFSET_U) | (decoded->loadstore << OFFSET_L) |
      (decoded->regn << OFFSET_RN) | (decoded->regd << OFFSET_RD) |
      (decoded->offset << OFFSET_OFFSET);
}

decoded_instruction_t* singledatatransfer_decode(instruction_t *instruction) {
  decoded_instruction_t *decoded_instruction = calloc(sizeof(decoded_instruction_t), 1);
  return decoded_instruction;
}

int singledatatransfer_execute(decoded_instruction_t* decoded,
                                machine_t* machine) {
  return 0;
}
