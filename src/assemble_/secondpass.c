#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "secondpass.h"
#include "instructions.h"

#define NUM_INSTRUCTIONS 23

/*
 *  Performs the second pass on code written in [filecontents]
 *  [table] is a linked list of symbolTableEntry objects
 *  [filecontents] is an array of strings which should be converted to binary
 *    and written to [out]
 */
void secondpass(symbol *table, char **filecontents, unsigned char *out) {
  
  instruction *instructions = calloc(NUM_INSTRUCTIONS, sizeof(instruction));
  initInstructions(instructions);

  // for each instruction
  for(int i = 0; filecontents[i] != 0; i++) {

    // split instruction into mnonic and arguments
    char *mnemonic = strtok(filecontents[i], " ");
    char *args = strtok(NULL, " ");

    instruction *ins = getInstruction(instructions, mnemonic);
    
    // if command doesnt have any args throw an error
    if(ins == NULL) {
      printf("Error: instruction \"%s %s\" invalid or not implemented\n", 
          mnemonic, args);
      return;
    }

    int binary = ins->createBinary(args);
 
    unsigned char c1 = binary >> 24;
    unsigned char c2 = binary >> 16;
    unsigned char c3 = binary >> 8;
    unsigned char c4 = binary;
    
    out[4*i] = c1;
    out[4*i+1] = c2;
    out[4*i+2] = c3;
    out[4*i+3] = c4;

    free(ins);
    
  }
}

instruction *getInstruction(instruction* instructions, char *mnemonic) {
  for(int i = 0; i < NUM_INSTRUCTIONS; i++) {
    if(strcmp(mnemonic, instructions[i].mnemonic) == 0) {
      instruction *ins = malloc(sizeof(instruction));
      memcpy(ins, &instructions[i], sizeof(instruction));
      return ins;
    }
  }
  return NULL;
}

void initInstructions(instruction* instructions) {
  
  // set all data processing types + binary templates
  for(int i = 0; i <= 9; i++) {
    instructions[i].type = DATA_PROCESSING;
  }

  instructions[0].mnemonic = "add";
  instructions[1].mnemonic = "sub";
  instructions[2].mnemonic = "rsp";
  instructions[3].mnemonic = "and";
  instructions[4].mnemonic = "eor";
  instructions[5].mnemonic = "orr";
  
  instructions[6].mnemonic = "mov";
  instructions[6].binary |= 0x0000A001;
  instructions[6].createBinary = &mov;

  instructions[7].mnemonic = "tst";
  instructions[8].mnemonic = "teq";
  instructions[9].mnemonic = "cmp";

  instructions[10].mnemonic = "mul";
  instructions[11].mnemonic = "mla";

  instructions[12].mnemonic = "ldr";
  instructions[13].mnemonic = "str";
  instructions[14].mnemonic = "beq";
  instructions[15].mnemonic = "bne";
  instructions[16].mnemonic = "bge";
  instructions[17].mnemonic = "blt";
  instructions[18].mnemonic = "bgt";
  instructions[19].mnemonic = "ble";
  instructions[20].mnemonic = "b";

  instructions[21].mnemonic = "lsl";
  instructions[22].mnemonic = "andeq";
}
