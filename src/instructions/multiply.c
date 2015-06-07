#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate.h"
#include "../instructions.h"

instruction_t multiply_encode(decoded_instruction_t *decoded) {
  const int OFFSET_COND = 0x1C;
  const int OFFSET_A = 0x15;
  const int OFFSET_RD = 0x10;
  const int OFFSET_RN = 0xC;
  const int OFFSET_RS = 0x8;
  const int OFFSET_1001 = 0x4;
  const int OFFSET_RM = 0x0;
  const int COND = 14;
  return (COND << OFFSET_COND) | (decoded->accumulate << OFFSET_A) |
      (decoded->regd << OFFSET_RD) | (decoded->regn << OFFSET_RN) |
      (decoded->regs << OFFSET_RS) | (0x9 << OFFSET_1001) |
      (decoded->regm << OFFSET_RM);
}

decoded_instruction_t* multiply_decode(instruction_t *instruction) {
  decoded_instruction_t *decoded_instruction =
    calloc(sizeof(decoded_instruction_t), 1);
  decoded_instruction->kind       = MULTIPLY;
  decoded_instruction->cond       = (*instruction & 0xf0000000) >> 28;
  decoded_instruction->accumulate = (*instruction & 0x00200000) != 0;
  decoded_instruction->set        = (*instruction & 0x00100000) != 0;
  decoded_instruction->regd       = (*instruction & 0x000f0000) >> 16;
  decoded_instruction->regn       = (*instruction & 0x0000f000) >> 12;
  decoded_instruction->regs       = (*instruction & 0x00000f00) >> 8;
  decoded_instruction->regm       = (*instruction & 0x0000000f);
  return decoded_instruction;
}

int multiply_execute(decoded_instruction_t* decoded, machine_t* machine) {
  if (condition_met(decoded, machine) != 0) {
    machine->registers[decoded->regd] = machine->registers[decoded->regm]
                                        * machine->registers[decoded->regs];
    if (decoded->accumulate != 0) {
      machine->registers[decoded->regd] += machine->registers[decoded->regn];
    }
    set_bit(Z, machine, machine->registers[decoded->regd] == 0);
    set_bit(N, machine, (machine->registers[decoded->regd] & 0x80000000) != 0);
  }
  return 0;
}
