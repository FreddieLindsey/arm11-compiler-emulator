#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate.h"
#include "../instructions.h"

instruction_t branch_encode(decoded_instruction_t *decoded) {
  int OFFSET_COND = 0x1C;
  int OFFSET_101 = 0x19;
  int OFFSET_OFFSET = 0;
  return (decoded->cond << OFFSET_COND) | (0x5 << OFFSET_101) |
      ((decoded->offset & ((1 << 24) - 1)) << OFFSET_OFFSET);
}

decoded_instruction_t* branch_decode(instruction_t *instruction) {
  decoded_instruction_t *decoded_instruction = calloc(sizeof(decoded_instruction_t), 1);
  return decoded_instruction;
}

int branch_execute(decoded_instruction_t* decoded, machine_t* machine) {
  return 0;
}
