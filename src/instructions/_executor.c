#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../emulate.h"
#include "../instructions.h"

int execute_instruction(decoded_instruction_t* decoded, machine_t* machine) {
  switch(decoded->kind) {
    case BRANCH: 
      branch_execute(decoded, machine);
      break;
    case SINGLE_DATA_TRANSFER: 
      sdt_execute(decoded, machine);
      break;
    case MULTIPLY: 
      multiply_execute(decoded, machine);
      break;
    case DATA_PROCESS: 
      dataprocess_execute(decoded, machine);
      break;
    default: /* Inform of unsupported instruction */
      if (verbose) 
        printf("Incorrect instruction received.\n");
  }
}
