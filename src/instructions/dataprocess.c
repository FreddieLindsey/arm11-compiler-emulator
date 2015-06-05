#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate.h"
#include "../instructions.h"

/*
 *  Functions to encode the different types of instruction
 */
instruction_t dataprocess_encode(decoded_instruction_t *decoded) {
  const int OFFSET_COND = 0x1C;
  const int OFFSET_I = 0x19;
  const int OFFSET_OPCODE = 0x15;
  const int OFFSET_S = 0x14;
  const int OFFSET_RN = 0x10;
  const int OFFSET_RD = 0xC;
  const int OFFSET_OPERAND2 = 0x0;
  const int COND = 14;
  return (COND << OFFSET_COND) | (decoded->immediate << OFFSET_I) |
      (decoded->opcode << OFFSET_OPCODE) | (decoded->set << OFFSET_S) |
      (decoded->regn << OFFSET_RN) | (decoded->regd << OFFSET_RD) |
      (decoded->operand2 << OFFSET_OPERAND2);
}

decoded_instruction_t* dataprocess_decode(instruction_t *instruction) {
  decoded_instruction_t *decoded_instruction = calloc(sizeof(decoded_instruction_t), 1);
  return decoded_instruction;
}

int dataprocess_execute(decoded_instruction_t* decoded, machine_t* machine) {
  return 0;
}
