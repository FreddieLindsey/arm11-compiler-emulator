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
  const int OFFSET_COND = 0x1C;
  const int OFFSET_A = 0x15;
  const int OFFSET_RD = 0x10;
  const int OFFSET_RN = 0xC;
  const int OFFSET_RS = 0x8;
  const int OFFSET_1001 = 0x4;
  const int OFFSET_RM = 0x0; 
  const int COND = 14;
  return (COND << OFFSET_COND) | (decoded->accumulate << OFFSET_A) |
      (decoded->regd << OFFSET_RD) | (decoded->regn << OFFSET_RN) |
      (decoded->regs << OFFSET_RS) | (0x9 << OFFSET_1001) |
      (decoded->regm << OFFSET_RM);
}

instruction_t encode_single_data_transfer(decoded_instruction_t *decoded) {
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

instruction_t encode_branch(decoded_instruction_t *decoded) {
  int OFFSET_COND = 0x1C;
  int OFFSET_101 = 0x19;
  int OFFSET_OFFSET = 0;
  return (decoded->cond << OFFSET_COND) | (0x5 << OFFSET_101) | 
      ((decoded->offset & ((1 << 24) - 1)) << OFFSET_OFFSET);
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
    case ANDEQ: return 0;
    default: return -1;
  }
}
