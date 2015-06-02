#ifndef _DECODER
#define _DECODER

#include "emulate.h"

/*
 *  TYPEDEFS / STRUCTS / UNIONS
 */

typedef enum instruction_k {
  dataproc = 0, multiply = 1, singledatatransfer = 2, branch = 3, special = -1
} instruction_kt;

typedef struct decoded_instruction {
  instruction_kt kind;
  uint8_t cond;
  uint8_t opcode;
  uint8_t immediate;
  uint8_t set;
  uint8_t prepost;
  uint8_t up;
  uint8_t loadstore;
  instruction_t regd;
  instruction_t regs1;
  instruction_t regs2;
  instruction_t regs3;
  instruction_t other;
} decoded_instruction_t ;

/*
 *  FUNCTION PROTOTYPES
 */

/*  EXECUTOR */
int instruction_execute(decoded_instruction_t *decoded, machine_t *machine);

/*  ENCODER */
instruction_t instruction_encode(decoded_instruction_t* decoded);

/*  DECODER */
decoded_instruction_t* instruction_decode(instruction_t instruction);

/*  FETCH */
instruction_t fetch_instruction(machine_t *machine);
instruction_t fetch_instruction_pos_format(machine_t *machine, 
  addressable_t mempos);
instruction_t fetch_instruction_pos(machine_t *machine, addressable_t mempos);

#endif
