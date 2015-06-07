#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate.h"
#include "../instructions.h"

instruction_t singledatatransfer_encode(decoded_instruction_t *decoded) {
  const int OFFSET_COND = 0x1C;
  const int OFFSET_1 = 0x1A;
  const int OFFSET_I = 0x19;
  const int OFFSET_P = 0x18;
  const int OFFSET_U = 0x17;
  const int OFFSET_L = 0x14;
  const int OFFSET_RN = 0x10;
  const int OFFSET_RD = 0xC;
  const int OFFSET_OFFSET = 0x0;
  const int COND = 14;
  return (COND << OFFSET_COND) | (0x1 << OFFSET_1) |
      (decoded->immediate << OFFSET_I) | (decoded->prepost << OFFSET_P) |
      (decoded->up << OFFSET_U) | (decoded->loadstore << OFFSET_L) |
      (decoded->regn << OFFSET_RN) | (decoded->regd << OFFSET_RD) |
      (decoded->offset << OFFSET_OFFSET);
}

decoded_instruction_t* singledatatransfer_decode(instruction_t *instruction) {
  decoded_instruction_t *decoded_instruction =
    calloc(sizeof(decoded_instruction_t), 1);
  decoded_instruction->kind       = SINGLE_DATA_TRANSFER;
  decoded_instruction->cond       = (*instruction & 0xf0000000) >> 28;
  decoded_instruction->immediate  = (*instruction & 0x02000000) != 0;
  decoded_instruction->prepost    = (*instruction & 0x01000000) != 0;
  decoded_instruction->up         = (*instruction & 0x00800000) != 0;
  decoded_instruction->loadstore  = (*instruction & 0x00100000) != 0;
  decoded_instruction->regn       = (*instruction & 0x000f0000) >> 16;
  decoded_instruction->regd       = (*instruction & 0x0000f000) >> 12;
  decoded_instruction->offset     = (*instruction & 0x00000fff);
  return decoded_instruction;
}

instruction_t fetch_instruction_mem(machine_t *machine, addressable_t mempos) {
  /* Initialise the instruction to 0, get the memory pointer */
  instruction_t instruction = 0;
  memchunk_t *memposptr =
    &machine->memory[mempos + sizeof(instruction_t) - 1];

  /* Compile the instruction */
  int j;
  for (j = 0; j < sizeof(instruction_t); ++j) {
    instruction |=
      *(memposptr - j) << ((sizeof(instruction_t) - j - 1) * CHAR_BIT);
  }
  return instruction;
}

void store_instruction_mem(machine_t *machine, instruction_t *instruction,
                            addressable_t mempos) {
  printf("Storing instruction:\t0x%08x\nin memory:\t0x%08x\n", *instruction, mempos);
  /* Initialise the instruction to 0, get the memory pointer */
  memchunk_t *memposptr =
    &machine->memory[mempos];

  /* Compile the instruction */
  int j;
  for (j = 0; j < sizeof(instruction_t); ++j) {
    *(memposptr + j) =
      (*instruction >> j * CHAR_BIT)
      & 0x000000ff;
  }
}

int offsetregister(decoded_instruction_t* decoded, machine_t* machine,
                    instruction_t offsetval, int neg) {
  addressable_t temp = machine->registers[decoded->regn];
  if (neg != 0 && temp < offsetval) {
    printf("Error: Out of bounds memory access at address\n");
    return -1;
  }
  temp += neg != 0 ? - offsetval : offsetval;
  if (temp > machine->memsize) {
    printf("Error: Out of bounds memory access at address 0x%08x\n",
            temp);
    return -1;
  }
  machine->registers[decoded->regn] = temp;
  return 0;
}

int singledatatransfer_execute(decoded_instruction_t* decoded,
                                machine_t* machine) {
  if (condition_met(decoded, machine) != 0) {

    instruction_t offsetvalue =
      (decoded->immediate == 0) ?
      decoded->offset :
      get_operand(decoded->offset, 0, machine);
    int neg = (decoded->up != 0) ? 0 : 1;

    //offset register, load data then reset register
    int ok = 1;
    if (decoded->prepost != 0) ok = offsetregister(decoded, machine,
                                                    offsetvalue, neg);
    if (ok != 0) return 1;
    if (decoded->loadstore != 0) {
      machine->registers[decoded->regd] =
        fetch_instruction_mem(machine, machine->registers[decoded->regn]);
    } else {
      store_instruction_mem(machine, &(machine->registers[decoded->regd]),
                            machine->registers[decoded->regn]);
    }
    offsetregister(decoded, machine, offsetvalue,
      ((decoded->prepost != 0) ? neg == 0 : neg));
  }
  return 1;
}
