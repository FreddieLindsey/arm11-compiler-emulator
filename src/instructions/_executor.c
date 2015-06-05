#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../emulate.h"
#include "../instructions.h"

int instruction_execute(decoded_instruction_t* decoded, machine_t* machine) {
  switch(decoded->kind) {
    case BRANCH:
      return branch_execute(decoded, machine);
      break;
    case SINGLE_DATA_TRANSFER:
      return singledatatransfer_execute(decoded, machine);
      break;
    case MULTIPLY:
      return multiply_execute(decoded, machine);
      break;
    case DATA_PROCESS:
      return dataprocess_execute(decoded, machine);
      break;
    default: /* Inform of unsupported instruction */
      printf("Incorrect instruction received.\n");
      return 0;
  }
}

int condition_met(decoded_instruction_t* decoded, machine_t* machine) {
  switch(decoded->cond) {
    default:
      printf("Condition %08i not understood!\n", decoded->cond);
      return -1;
  }
}
