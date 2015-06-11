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

void branch_decode(pipeline_t *pipeline) {
  pipeline->decoded->kind = BRANCH;
  pipeline->decoded->cond = (*pipeline->fetched & 0xf0000000) >> 28;
  pipeline->decoded->offset = (*pipeline->fetched & 0x00ffffff) << 2;
  if ((pipeline->decoded->offset & 0x02000000) != 0) {
    pipeline->decoded->offset |= 0xfc000000;
  }
}

int branch_execute(machine_t* machine) {
  if (condition_met(machine->pipeline->decoded, machine) != 0) {
    *(machine->pc) += machine->pipeline->decoded->offset;
    if (machine->pipeline->decoded != NULL) free(machine->pipeline->decoded);
    machine->pipeline->decoded = NULL;
    if (machine->pipeline->fetched != NULL) free(machine->pipeline->fetched);
    machine->pipeline->fetched = NULL;
  }
  return 1;
}
