#ifndef _DECODER
#define _DECODER

#include "emulate.h"

int instruction_execute(char** instruction_decoded, machine_t *machine);
char** instruction_decode(instruction_t instruction);

#endif
