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

void dataprocess_decode(pipeline_t *pipeline) {
  pipeline->decoded->kind       = DATA_PROCESS;
  pipeline->decoded->cond       = (*pipeline->fetched & 0xf0000000) >> 28;
  pipeline->decoded->immediate  = (*pipeline->fetched & 0x02000000) != 0;
  pipeline->decoded->opcode     = (*pipeline->fetched & 0x01e00000) >> 21;
  pipeline->decoded->set        = (*pipeline->fetched & 0x00100000) != 0;
  pipeline->decoded->regn       = (*pipeline->fetched & 0x000f0000) >> 16;
  pipeline->decoded->regd       = (*pipeline->fetched & 0x0000f000) >> 12;
  pipeline->decoded->operand2   = (*pipeline->fetched & 0x00000fff);
}

int dataprocess_execute(machine_t* machine) {
  if (condition_met(machine->pipeline->decoded, machine) != 0) {
    switch(machine->pipeline->decoded->opcode) {
      case and:
        return and_execute(machine->pipeline->decoded, machine);
      case eor:
        return eor_execute(machine->pipeline->decoded, machine);
      case sub:
        return sub_execute(machine->pipeline->decoded, machine);
      case rsb:
        return rsb_execute(machine->pipeline->decoded, machine);
      case add:
        return add_execute(machine->pipeline->decoded, machine);
      case tst:
        return tst_execute(machine->pipeline->decoded, machine);
      case teq:
        return teq_execute(machine->pipeline->decoded, machine);
      case cmp:
        return cmp_execute(machine->pipeline->decoded, machine);
      case orr:
        return orr_execute(machine->pipeline->decoded, machine);
      case mov:
        return mov_execute(machine->pipeline->decoded, machine);
      default:
        printf("Unsupported data process instruction:\t0x%04x\n",
                  machine->pipeline->decoded->opcode);
    }
  }
  return 1;
}

static int and_execute(decoded_instruction_t* decoded, machine_t* machine) {
  machine->registers[decoded->regd] =
    machine->registers[decoded->regn] &
    get_operand(decoded->operand2, decoded->immediate, machine, decoded->set);
  if (decoded->set) {
    set_bit(N, machine,
        (machine->registers[decoded->regn] & 0x80000000) != 0);
    set_bit(Z, machine, machine->registers[decoded->regn] == 0);
    set_bit(C, machine,
        (machine->registers[decoded->regn] &
        get_operand(decoded->operand2, decoded->immediate, machine, decoded->set)) >
        0xffffffff);

  }
  return 1;
}

static int eor_execute(decoded_instruction_t* decoded, machine_t* machine) {
  machine->registers[decoded->regd] =
    machine->registers[decoded->regn] ^
    get_operand(decoded->operand2, decoded->immediate, machine, decoded->set);
  return 1;
}

static int sub_execute(decoded_instruction_t* decoded, machine_t* machine) {
  instruction_t operand = get_operand(decoded->operand2, decoded->immediate, machine, decoded->set);
  machine->registers[decoded->regd] =
    machine->registers[decoded->regn] -
    operand;
  return 1;
}

static int rsb_execute(decoded_instruction_t* decoded, machine_t* machine) {
  machine->registers[decoded->regd] =
  get_operand(decoded->operand2, decoded->immediate, machine, decoded->set) -
    machine->registers[decoded->regn];
  return 1;
}

static int add_execute(decoded_instruction_t* decoded, machine_t* machine) {
  machine->registers[decoded->regd] =
    machine->registers[decoded->regn] +
    get_operand(decoded->operand2, decoded->immediate, machine, decoded->set);
  if (decoded->set) {
    set_bit(N, machine, (machine->registers[decoded->regd] & 0x80000000) != 0);
    set_bit(Z, machine, machine->registers[decoded->regd] != 0);
    set_bit(C, machine,
      (machine->registers[decoded->regn] +
      get_operand(decoded->operand2, decoded->immediate, machine, decoded->set))
      > 0xffffffff);
  }
  return 1;
}

static int tst_execute(decoded_instruction_t* decoded, machine_t* machine) {
  instruction_t test =
    machine->registers[decoded->regn] &
    get_operand(decoded->operand2, decoded->immediate, machine, decoded->set);
  if (decoded->set) {
    if (test == 0) set_bit(Z, machine, 1);
  }
  return 1;
}

static int teq_execute(decoded_instruction_t* decoded, machine_t* machine) {
  instruction_t test =
    machine->registers[decoded->regn] ^
    get_operand(decoded->operand2, decoded->immediate, machine, decoded->set);
  if (test == 0) set_bit(Z, machine, 1);
  return 1;
}

static int cmp_execute(decoded_instruction_t* decoded, machine_t* machine) {
  instruction_t test =
    machine->registers[decoded->regn] -
    get_operand(decoded->operand2, decoded->immediate, machine, decoded->set);
  if (decoded->set) {
    set_bit(N, machine, (test & 0x80000000) != 0);
    set_bit(Z, machine, test == 0);
    set_bit(C, machine,
      get_operand(decoded->operand2, decoded->immediate, machine, decoded->set) <=
      machine->registers[decoded->regn]);
  }
  return 1;
}

static int orr_execute(decoded_instruction_t* decoded, machine_t* machine) {
  machine->registers[decoded->regd] =
    machine->registers[decoded->regn] |
    get_operand(decoded->operand2, decoded->immediate, machine, decoded->set);
  return 1;
}

static int mov_execute(decoded_instruction_t* decoded, machine_t* machine) {
  machine->registers[decoded->regd] =
    get_operand(decoded->operand2, decoded->immediate, machine, decoded->set);
  return 1;
}
