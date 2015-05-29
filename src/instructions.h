#ifndef _DECODER
#define _DECODER

#include "emulate.h"

/*  EXECUTOR */
int instruction_execute(char** instruction_decoded, machine_t *machine);

/*  ENCODER */


/*  DECODER */
char** instruction_decode(instruction_t instruction);

/*  FETCH */
instruction_t fetch_instruction(machine_t *machine);
instruction_t fetch_instruction_pos_be(machine_t *machine, addressable_t mempos);
instruction_t fetch_instruction_pos(machine_t *machine, addressable_t mempos);

#endif
