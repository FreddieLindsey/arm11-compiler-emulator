#ifndef _INSTRUCTIONS
#define _INSTRUCTIONS

#include <stdint.h>
#include <stdio.h>

/*
 *  TYPEDEFS / STRUCTS / UNIONS
 */

/*  GENERAL */
typedef uint8_t memchunk_t;
typedef uint32_t addressable_t;
typedef uint32_t instruction_t;

/*  CPSR BITS */
typedef enum cpsr_bit {
  N = 31, Z = 30, C = 29, V = 28
} cpsr_bit_t;

/*  CONDITIONS */
typedef enum condition_k {
  eq = 0, ne = 1, ge = 10, lt = 11, gt = 12, le = 13, al = 14
} condition_kt;

/*  SHIFT TYPES */
typedef enum shift_k {
  lsl = 0, lsr = 1, asr = 2, ror = 3
} shift_t;

/* DATA PROCESS OPCODES */
typedef enum opcode {
  and = 0, eor = 1, sub = 2, rsb = 3, add = 4,
  tst = 8, teq = 9, cmp = 10, orr = 12, mov = 13
} opcode_t;

/*  INSTRUCTION TYPES */
typedef enum instruction_k {
  DATA_PROCESS, MULTIPLY, SINGLE_DATA_TRANSFER, BRANCH, LSL, ANDEQ
} instruction_kt;

/*  DECODED INSTRUCTION */
typedef struct decoded_instruction {
  instruction_kt kind;
  condition_kt cond;
  opcode_t opcode;
  uint8_t immediate;
  uint8_t accumulate;
  uint8_t set;
  uint8_t prepost;
  uint8_t up;
  uint8_t loadstore;
  uint8_t regd;
  uint8_t regn;
  instruction_t operand2;
  uint8_t regs;
  uint8_t regm;
  int32_t offset;
} decoded_instruction_t;

/*  PIPELINE */
typedef struct pipeline {
  decoded_instruction_t* decoded;
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

/*  EXECUTOR (include auxilary functions in source) */
/*  Return 1 when not successful, -1 to halt, 0 for OK */
int instruction_execute(decoded_instruction_t* decoded, machine_t* machine);
int condition_met(decoded_instruction_t* decoded, machine_t* machine);
void set_bit(cpsr_bit_t bit, machine_t* machine);
int get_bit(cpsr_bit_t bit, machine_t* machine);
instruction_t* get_operand(instruction_t operand, uint8_t immediate);

/*  ENCODER */
instruction_t instruction_encode(decoded_instruction_t* decoded);

/*  DECODER */
decoded_instruction_t* instruction_decode(instruction_t *instruction);
void print_decoded(decoded_instruction_t decoded);

/*  FETCH */
instruction_t fetch_instruction(machine_t *machine);
instruction_t fetch_instruction_pos_format(machine_t *machine,
  addressable_t mempos);
instruction_t fetch_instruction_pos(machine_t *machine, addressable_t mempos);

/*  DATA_PROCESS */
instruction_t dataprocess_encode(decoded_instruction_t *decoded);
decoded_instruction_t* dataprocess_decode(instruction_t *instruction);
int dataprocess_execute(decoded_instruction_t* decoded, machine_t* machine);

/*  MULTIPLY */
instruction_t multiply_encode(decoded_instruction_t *decoded);
decoded_instruction_t* multiply_decode(instruction_t *instruction);
int multiply_execute(decoded_instruction_t* decoded, machine_t* machine);

/*  SINGLE_DATA_TRANSFER */
instruction_t singledatatransfer_encode(decoded_instruction_t *decoded);
decoded_instruction_t* singledatatransfer_decode(instruction_t *instruction);
int singledatatransfer_execute(decoded_instruction_t* decoded, machine_t* machine);

/*  BRANCH */
instruction_t branch_encode(decoded_instruction_t *decoded);
decoded_instruction_t* branch_decode(instruction_t *instruction);
int branch_execute(decoded_instruction_t* decoded, machine_t* machine);

/*  LSL */

/*  ANDEQ */

#endif
