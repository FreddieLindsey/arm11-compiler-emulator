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

void multiply_decode(pipeline_t *pipeline) {
  pipeline->decoded->kind       = MULTIPLY;
  pipeline->decoded->cond       = (*pipeline->fetched & 0xf0000000) >> 28;
  pipeline->decoded->accumulate = (*pipeline->fetched & 0x00200000) != 0;
  pipeline->decoded->set        = (*pipeline->fetched & 0x00100000) != 0;
  pipeline->decoded->regd       = (*pipeline->fetched & 0x000f0000) >> 16;
  pipeline->decoded->regn       = (*pipeline->fetched & 0x0000f000) >> 12;
  pipeline->decoded->regs       = (*pipeline->fetched & 0x00000f00) >> 8;
  pipeline->decoded->regm       = (*pipeline->fetched & 0x0000000f);
}

int multiply_execute(machine_t* machine) {
  if (condition_met(machine->pipeline->decoded, machine) != 0) {
    machine->registers[machine->pipeline->decoded->regd] =
    machine->registers[machine->pipeline->decoded->regm]
    * machine->registers[machine->pipeline->decoded->regs];
    if (machine->pipeline->decoded->accumulate != 0) {
      machine->registers[machine->pipeline->decoded->regd] +=
      machine->registers[machine->pipeline->decoded->regn];
    }
    set_bit(Z, machine,
              machine->registers[machine->pipeline->decoded->regd] == 0);
    set_bit(N, machine,
              (machine->registers[machine->pipeline->decoded->regd]
              & 0x80000000) != 0);
  }
  return 0;
}
