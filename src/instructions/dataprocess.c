#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../emulate.h"
#include "../instructions.h"

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
      case add:
        printf("Adding\n");
        machine->registers[decoded->regd] =
          machine->registers[decoded->regn] &
          (get_operand(decoded->operand2, decoded->immediate));
        break;
      case eor:
        machine->registers[decoded->regd] =
          machine->registers[decoded->regn] ^
          (get_operand(decoded->operand2, decoded->immediate));
        break;
      case mov:
        printf( "Moving\nReg D:\t\t0x%8x\nOperand:\t0x8x\n", get_operand(decoded->operand2, decoded->immediate));
        machine->registers[decoded->regd] =
          (get_operand(decoded->operand2, decoded->immediate));
        break;
      default:
        printf("Unsupported data process instruction:\t0x%04x\n",
                  decoded->opcode);
    }
  }
  return 1;
}
