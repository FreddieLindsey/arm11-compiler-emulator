#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../emulate.h"
#include "../instructions.h"

static void shift(shift_t shift_, instruction_t* operand, machine_t *machine, int set);
static void rotate_right(instruction_t* operand, machine_t *machine, int set);
static void logical_right(instruction_t* operand, machine_t *machine, int set);
static void logical_left(instruction_t* operand, machine_t *machine, int set);
static void arithmetic_right(instruction_t* operand, machine_t *machine, int set);

/*
 *  Returns -1 iff the instruction is halt.
 *  Returns  0 otherwise.
 */
int instruction_execute(decoded_instruction_t* decoded, machine_t* machine) {
  if (decoded->kind != HALT) {
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
  return -1;
}

/*
 *  Returns  1 if condition met
 *  Returns  0 if condition NOT met
 *  Returns -1 if condition NOT understood
 */
int condition_met(decoded_instruction_t* decoded, machine_t* machine) {
  switch(decoded->cond) {
    case eq:
      return get_bit(Z, machine);
    case ne:
      return get_bit(Z, machine) == 0;
    case ge:
      return get_bit(N, machine) == get_bit(V, machine);
    case lt:
      return get_bit(N, machine) != get_bit(V, machine);
    case gt:
      return !get_bit(Z, machine) && get_bit(N, machine) == get_bit(V, machine);
    case le:
      return get_bit(Z, machine) || get_bit(N, machine) != get_bit(V, machine);
    case al:
      return 1;
    default:
      printf("Condition %08i not understood!\n", decoded->cond);
      return -1;
  }
}

void set_bit(cpsr_bit_t bit, machine_t* machine, int boolean) {
  switch(bit) {
    case N:
      if (boolean != 0) {
        *(machine->cpsr) |= 0x80000000;
      } else {
        *(machine->cpsr) &= 0x7fffffff;
      }
      break;
    case Z:
      if (boolean != 0) {
        *(machine->cpsr) |= 0x40000000;
      } else {
        *(machine->cpsr) &= 0xbfffffff;
      }
      break;
    case C:
      if (boolean != 0) {
        *(machine->cpsr) |= 0x20000000;
      } else {
        *(machine->cpsr) &= 0xdfffffff;
      }
      break;
    case V:
      if (boolean != 0) {
        *(machine->cpsr) |= 0x10000000;
      } else {
        *(machine->cpsr) &= 0xefffffff;
      }
      break;
    default:
      printf("Bit given is unsupported\n");
  }
}

int get_bit(cpsr_bit_t bit, machine_t* machine) {
  switch(bit) {
    case N:
      return (*(machine->cpsr) & 0x80000000) != 0;
    case Z:
      return (*(machine->cpsr) & 0x40000000) != 0;
    case C:
      return (*(machine->cpsr) & 0x20000000) != 0;
    case V:
      return (*(machine->cpsr) & 0x10000000) != 0;
    default:
      printf("Bit given is unsupported\n");
      return -1;
  }
}

instruction_t get_operand(instruction_t operand,
                          uint8_t immediate,
                          machine_t* machine, int set) {
  instruction_t operand_o = 0;
  if (immediate != 0) {
    operand_o = (operand & 0x000000ff);
    uint8_t rotate_ = (operand & 0x00000f00) >> 8;
    while (rotate_ > 0) {
      shift(ror, &operand_o, machine, set);
      shift(ror, &operand_o, machine, set);
      --rotate_;
    }
  } else {
    if ((operand & 0x00000010) != 0) {
      uint8_t rotate_ =
        machine->registers[(operand & 0x00000f00) >> 7] &
        0x000000ff;
      shift_t shift_ = (operand & 0x00000060) >> 5;
      operand_o = machine->registers[(operand & 0x0000000f)];
      while (rotate_ > 0) {
        shift(shift_, &operand_o, machine, set);
        --rotate_;
      }
    } else {
      instruction_t rotate_ = (operand & 0x00000f80) >> 7;
      shift_t shift_ = (operand & 0x00000060) >> 5;
      operand_o = machine->registers[(operand & 0x0000000f)];
      while (rotate_ > 0) {
        shift(shift_, &operand_o, machine, set);
        --rotate_;
      }
    }
  }
  return operand_o;
}

static void shift(shift_t shift_, instruction_t* operand,
                  machine_t *machine, int set) {
  switch(shift_) {
    case lsl:
      logical_left(operand, machine, set);
      break;
    case lsr:
      logical_right(operand, machine, set);
      break;
    case asr:
      arithmetic_right(operand, machine, set);
      break;
    case ror:
      rotate_right(operand, machine, set);
      break;
    default:
      printf("Unsupported shift type\n");
  }
}

static void rotate_right(instruction_t* operand, machine_t *machine, int set) {
  instruction_t rotation = (*operand & 0x00000001) << 31;
  *operand = (*operand >> 1) | rotation;
}

static void logical_right(instruction_t* operand, machine_t *machine, int set) {
  if (set != 0) set_bit(C, machine, (*operand & 0x00000001) != 0);
  *operand = *operand >> 1;
}

static void logical_left(instruction_t* operand, machine_t *machine, int set) {
  if (set != 0) set_bit(C, machine, (*operand & 0x80000000) != 0);
  *operand = *operand << 1;
}

static void arithmetic_right(instruction_t* operand,
                             machine_t *machine, int set) {
  if (set != 0) set_bit(C, machine, (*operand & 0x00000001) != 0);
  instruction_t rotation = *operand | 0x7fffffff;
  *operand = (*operand >> 1) & rotation;
}
