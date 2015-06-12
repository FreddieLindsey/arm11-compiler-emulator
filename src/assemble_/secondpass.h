#ifndef SECONDPASS_H 
#define SECONDPASS_H

#include "assemble.h"
#include "../instructions.h"

typedef struct instruction_str {
  // instruction mnemonic (mov, add, and ...)
  char *mnemonic;

  // function to create binary
  decoded_instruction_t *(*buildInstruction)(); 

  // type of instruction (see enum above);
  instruction_kt type;
} instruction_str_t;

void secondpass(symbol_t *table, char **filecontents, output_data_t *out);
void initInstructions(instruction_str_t* instructions);
instruction_str_t *getInstruction(instruction_str_t* instructions,
    char *mnemonic);

#endif
