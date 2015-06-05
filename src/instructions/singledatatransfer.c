#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate.h"
#include "../instructions.h"

instruction_t singledatatransfer_encode(decoded_instruction_t *decoded) {
  return 0;
}

decoded_instruction_t* singledatatransfer_decode(instruction_t *instruction) {
  decoded_instruction_t *decoded_instruction = calloc(sizeof(decoded_instruction_t), 1);
  return decoded_instruction;
}

int singledatatransfer_execute(decoded_instruction_t* decoded,
                                machine_t* machine) {
  return 0;
}
