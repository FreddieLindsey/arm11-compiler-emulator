#ifndef SECONDPASS_H 
#define SECONDPASS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "assemble.h"

typedef enum instructionType {
  DATA_PROCESSING, MULTIPLY, SINGLE_DATA_TRANSFER, BRANCH, SPECIAL
} instructionType;

typedef struct instruction instruction;
struct instruction {
  // instruction mnemonic (mov, add, and ...)
  char *mnemonic;

  // function to create binary
  uint32_t (*createBinary)(char **args); 
};

void secondpass(symbol *table, char **filecontents, unsigned char *out);
void initInstructions(instruction* instructions);
instruction *getInstruction(instruction* instructions, char *mnemonic);

#endif
