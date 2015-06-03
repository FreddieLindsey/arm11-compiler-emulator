#ifndef _DECODER
#define _DECODER

#include <stdint.h>
#include <stdio.h>

/*
 *  TYPEDEFS / STRUCTS / UNIONS
 */

/*  GENERAL */
typedef uint8_t memchunk_t;
typedef uint32_t addressable_t;
typedef uint32_t instruction_t;

/* INSTRUCTION TYPES */
typedef enum instruction_k {
  DATA_PROCESS, MULTIPLY, SINGLE_DATA_TRANSFER, BRANCH, SPECIAL
} instruction_kt;

/* INSTRUCTION */
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

/*  PIPELINE */
typedef struct pipeline {
  decoded_instruction_t decoded;
  instruction_t fetched;
} pipeline_t;

/*  MACHINE */
typedef struct machine {
  memchunk_t *memory;
  addressable_t memsize;
  instruction_t *registers;
  addressable_t regcount;
  instruction_t *pc;
  instruction_t *cpsr;
  pipeline_t *pipeline;
} machine_t;

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
