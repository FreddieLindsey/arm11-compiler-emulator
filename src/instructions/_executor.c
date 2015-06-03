#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../emulate.h"
#include "../instructions.h"

int execute_instruction(decoded_instruction_t* decoded, machine_t* machine) {
  switch(decoded->kind) {
    case branch: 
      branch_execute(decoded, machine);
      break;
    case singledatatransfer: 
      sdt_execute(decoded, machine);
      break:
    case multiply: 
      multiply_execute(decoded, machine);
      break:
    case dataprocess: 
      dataprocess_execute(decoded, machine);
      break:
    default: /* Inform of unsupported instruction */
      if (verbose) 
        printf("Incorrect instruction received.\n");
  }
}