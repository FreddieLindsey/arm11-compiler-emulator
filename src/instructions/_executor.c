#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../emulate.h"
#include "../instructions.h"

static void rotate(instruction_t* operand, uint8_t rotate);

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
    case eq: return get_bit(Z, machine);
    case ne: return get_bit(Z, machine) ? 0: 1;

    default:
      printf("Condition %08i not understood!\n", decoded->cond);
      return -1;
  }
}

void set_bit(cpsr_bit_t bit, machine_t* machine) {
  switch(bit) {
    default:
      printf("Bit given is unsupported");
  }
}

int get_bit(cpsr_bit_t bit, machine_t* machine) {
  switch(bit) {
    default:
      printf("Bit given is unsupported");
      return -1;
  }
}

instruction_t* get_operand(instruction_t operand, uint8_t immediate) {
  instruction_t *operand_o = 0;
  if (immediate != 0) {
    *operand_o = (operand & 0x000000ff) << 24;
    uint8_t rotate_ = (operand & 0x00000f00) >> 8;
    rotate(operand_o, rotate_);
  } else {

  }
  return operand_o;
}

static void rotate(instruction_t* operand, uint8_t rotate) {
  int i;
  for (i = 0; i < rotate * 2; ++i) {
    instruction_t rotation = (*operand & 0x00000001) << 31;
    *operand = (*operand >> 1) | rotation;
  }
}
