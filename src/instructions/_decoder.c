#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate.h"
#include "../instructions.h"

static void halt_decode(decoded_instruction_t *decoded_instruction);

void instruction_decode(pipeline_t *pipeline) {
  int is_branch   = (*pipeline->fetched & 0x0f000000) == 0x0a000000;
  int is_sdt      = (*pipeline->fetched & 0x0c000000) == 0x04000000;
  int is_mult     = (*pipeline->fetched & 0x0e000000) == 0 &&
                    (*pipeline->fetched & 0x000000f0) == 0x00000090;
  int is_datproc  = *pipeline->fetched != 0;
  if (is_branch) {
    branch_decode(pipeline);
    return;
  } else if (is_sdt) {
    singledatatransfer_decode(pipeline);
    return;
  } else if (is_mult) {
    multiply_decode(pipeline);
    return;
  } else if (is_datproc) {
    dataprocess_decode(pipeline);
    return;
  }
  halt_decode(pipeline->decoded);
}

static void halt_decode(decoded_instruction_t *decoded_instruction) {
  decoded_instruction->kind       = HALT;
  decoded_instruction->cond       = 0;
  decoded_instruction->immediate  = 0;
  decoded_instruction->opcode     = 0;
  decoded_instruction->set        = 0;
  decoded_instruction->regn       = 0;
  decoded_instruction->regd       = 0;
  decoded_instruction->operand2   = 0;
}
