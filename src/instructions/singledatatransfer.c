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

void loaddata(decoded_instruction_t* decoded, machine_t* machine) {
  instruction_t loaded = 0; 
  instruction_t address = machine->registers[decoded->regn] + 2;
  loaded = machine->memory[address - 1] << 24 |
           machine->memory[address - 2] << 16 |
           machine->memory[address - 3] << 8  |
           machine->memory[address - 4];
  machine->registers[decoded->regd] = loaded;
}

void storedata(decoded_instruction_t* decoded, machine_t* machine) {
  instruction_t value = machine->registers[decoded->regd];
  instruction_t address = machine->registers[decoded->regn] / 2;
  machine->memory[address + 3] = (value << 24);
  machine->memory[address + 2] = (value << 16) - (0 | (value << 24));
  machine->memory[address + 1] = (value << 8)  - (0 | (value << 16));
  machine->memory[address]     = value - (0 | (value << 8)); 
} 

void offsetregister(decoded_instruction_t* decoded, machine_t* machine,
                    int offsetval) {
  int temp = machine->registers[decoded->regn];
  if(-temp > offsetval || (temp + offsetval) > 65536) {
    printf("Offset to large causing invalid memory address");
  }
  temp += offsetval;
  machine->registers[decoded->regn] = temp;
}

int singledatatransfer_execute(decoded_instruction_t* decoded,
                                machine_t* machine) {
  if(condition_met(decoded, machine) != 0) {

    int offsetvalue =
      get_operand(decoded->offset, (decoded->immediate != 0),
                                     machine);
    offsetvalue = (decoded->up == 1) ? offsetvalue : -offsetvalue;

    if(decoded->loadstore != 0) {
      if(decoded->prepost != 0) {
        //offset register, load data then reset register
        offsetregister(decoded, machine, offsetvalue);
        loaddata(decoded, machine);
        offsetregister(decoded, machine, -offsetvalue);
      } else {
        //load data then offset register
        loaddata(decoded, machine);
        offsetregister(decoded, machine, offsetvalue);
      }
    } else {
      if(decoded->prepost != 0) {
        //offset register, store into memory then reset register
        offsetregister(decoded, machine, offsetvalue);
        storedata(decoded, machine);
        offsetregister(decoded, machine, -offsetvalue);
      } else {
        //store into memory then offset register
        storedata(decoded, machine);
        offsetregister(decoded, machine, offsetvalue);
      }
    }
  }
  return 1;
}
