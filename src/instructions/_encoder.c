#include "../instructions.h"

/*
 *  Functions to encode the different types of instruction 
 */
instruction_t encode_data_process(decoded_instruction_t *decoded) {
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

instruction_t encode_multiply(decoded_instruction_t *decoded) { 
  return 0;
}

instruction_t encode_single_data_transfer(decoded_instruction_t *decoded) {
  return 0;
}

instruction_t encode_branch(decoded_instruction_t *decoded) {
  return 0;
}

/*
 *  Encodes a instruction from a decoded instruction struct to its binary
 *  equivalent
 */
instruction_t instruction_encode(decoded_instruction_t *decoded) {
  switch(decoded->kind) {
    case DATA_PROCESS: return encode_data_process(decoded); 
    case MULTIPLY: return encode_multiply(decoded); 
    case SINGLE_DATA_TRANSFER: return encode_single_data_transfer(decoded); 
    case BRANCH: return encode_branch(decoded); 
    default: return -1;
  }
}
