#ifndef SECONDPASS_H 
#define SECONDPASS_H

#include "assemble.h"
#include "../instructions.h"

typedef struct instruction_str {
  // instruction mnemonic (mov, add, and ...)
  char *mnemonic;

  // function to create binary
  decoded_instruction_t *(*build_instruction)(); 

  // type of instruction (see enum above);
  instruction_kt type;
} instruction_str_t;

void secondpass(symbol_t *table, char **filecontents, output_data_t *out);
void init_instructions(instruction_str_t* instructions);
instruction_str_t *get_instruction(instruction_str_t* instructions,
    char *mnemonic);

#endif
