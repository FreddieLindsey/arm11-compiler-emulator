#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate.h"
#include "../instructions.h"

static int and_execute(decoded_instruction_t* decoded, machine_t* machine);
static int eor_execute(decoded_instruction_t* decoded, machine_t* machine);
static int sub_execute(decoded_instruction_t* decoded, machine_t* machine);
static int rsb_execute(decoded_instruction_t* decoded, machine_t* machine);
static int add_execute(decoded_instruction_t* decoded, machine_t* machine);
static int tst_execute(decoded_instruction_t* decoded, machine_t* machine);
static int teq_execute(decoded_instruction_t* decoded, machine_t* machine);
static int cmp_execute(decoded_instruction_t* decoded, machine_t* machine);
static int orr_execute(decoded_instruction_t* decoded, machine_t* machine);
static int mov_execute(decoded_instruction_t* decoded, machine_t* machine);

/*
 *  Functions to encode the different types of instruction
 */
instruction_t dataprocess_encode(decoded_instruction_t *decoded) {
  const int OFFSET_COND = 0x1C;
  const int OFFSET_I = 0x19;
  const int OFFSET_OPCODE = 0x15;
  const int OFFSET_S = 0x14;
  const int OFFSET_RN = 0x10;
  const int OFFSET_RD = 0xC;
  const int OFFSET_OPERAND2 = 0x0;
  const int COND = 14;
  return (COND << OFFSET_COND) | (decoded->immediate << OFFSET_I) |
      (decoded->opcode << OFFSET_OPCODE) | (decoded->set << OFFSET_S) |
      (decoded->regn << OFFSET_RN) | (decoded->regd << OFFSET_RD) |
      (decoded->operand2 << OFFSET_OPERAND2);
}

decoded_instruction_t* dataprocess_decode(instruction_t *instruction) {
  decoded_instruction_t *decoded_instruction =
    calloc(sizeof(decoded_instruction_t), 1);
  decoded_instruction->kind       = DATA_PROCESS;
  decoded_instruction->cond       = (*instruction & 0xf0000000) >> 28;
  decoded_instruction->immediate  = (*instruction & 0x02000000) != 0;
  decoded_instruction->opcode     = (*instruction & 0x01e00000) >> 21;
  decoded_instruction->set        = (*instruction & 0x00100000) != 0;
  decoded_instruction->regn       = (*instruction & 0x000f0000) >> 16;
  decoded_instruction->regd       = (*instruction & 0x0000f000) >> 12;
  decoded_instruction->operand2   = (*instruction & 0x00000fff);
  return decoded_instruction;
}

int dataprocess_execute(decoded_instruction_t* decoded, machine_t* machine) {
  if (condition_met(decoded, machine) != 0) {
    switch(decoded->opcode) {
      case and:
        return and_execute(decoded, machine);
      case eor:
        return eor_execute(decoded, machine);
      case sub:
        return sub_execute(decoded, machine);
      case rsb:
        return rsb_execute(decoded, machine);
      case add:
        return add_execute(decoded, machine);
      case tst:
        return tst_execute(decoded, machine);
      case teq:
        return teq_execute(decoded, machine);
      case cmp:
        return cmp_execute(decoded, machine);
      case orr:
        return orr_execute(decoded, machine);
      case mov:
        return mov_execute(decoded, machine);
      default:
        printf("Unsupported data process instruction:\t0x%04x\n",
                  decoded->opcode);
    }
  }
  return 1;
}

static int and_execute(decoded_instruction_t* decoded, machine_t* machine) {
  machine->registers[decoded->regd] =
    machine->registers[decoded->regn] &
    get_operand(decoded->operand2, decoded->immediate);
  return 1;
}

static int eor_execute(decoded_instruction_t* decoded, machine_t* machine) {
  machine->registers[decoded->regd] =
    machine->registers[decoded->regn] ^
    get_operand(decoded->operand2, decoded->immediate);
  return 1;
}

static int sub_execute(decoded_instruction_t* decoded, machine_t* machine) {
  machine->registers[decoded->regd] =
    machine->registers[decoded->regn] -
    get_operand(decoded->operand2, decoded->immediate);
  return 1;
}

static int rsb_execute(decoded_instruction_t* decoded, machine_t* machine) {
  machine->registers[decoded->regd] =
    get_operand(decoded->operand2, decoded->immediate) -
    machine->registers[decoded->regn];
  return 1;
}

static int add_execute(decoded_instruction_t* decoded, machine_t* machine) {
  machine->registers[decoded->regd] =
    machine->registers[decoded->regn] +
    get_operand(decoded->operand2, decoded->immediate);
  return 1;
}

static int tst_execute(decoded_instruction_t* decoded, machine_t* machine) {
  instruction_t test =
    machine->registers[decoded->regn] &
    get_operand(decoded->operand2, decoded->immediate);
  return 1;
}

static int teq_execute(decoded_instruction_t* decoded, machine_t* machine) {
  instruction_t test =
    machine->registers[decoded->regn] ^
    get_operand(decoded->operand2, decoded->immediate);
  return 1;
}

static int cmp_execute(decoded_instruction_t* decoded, machine_t* machine) {
  instruction_t test =
    machine->registers[decoded->regn] -
    get_operand(decoded->operand2, decoded->immediate);
  return 1;
}

static int orr_execute(decoded_instruction_t* decoded, machine_t* machine) {
  machine->registers[decoded->regd] =
    machine->registers[decoded->regn] |
    get_operand(decoded->operand2, decoded->immediate);
  return 1;
}

static int mov_execute(decoded_instruction_t* decoded, machine_t* machine) {
  machine->registers[decoded->regd] =
    get_operand(decoded->operand2, decoded->immediate);
  return 1;
}
