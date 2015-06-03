#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "secondpass.h"
#include "build_instruction.h"

#define NUM_INSTRUCTIONS 23

/*
 *    Performs the second pass on code written in [filecontents]
 *  [table] is a linked list of symbolTableEntry objects
 *  [filecontents] is an array of strings which should be converted to binary
 *  and written to [out]
 */
void secondpass(symbol *table, char **filecontents, unsigned char *out) {
  instruction_str_t *instructions = 
      calloc(NUM_INSTRUCTIONS, sizeof(instruction_str_t));
  initInstructions(instructions);

  // for each instruction
  for(int i = 0; filecontents[i] != 0; i++) {

    // find first space in string
    char *firstspace = strchr(filecontents[i], ' ');

    if(firstspace == NULL) {
      printf("Error: instruction \"%s\" expected arguments\n", 
          filecontents[i]);
      exit(EXIT_FAILURE);
    } 

    // split instruction into mnemonic and arguments
    *firstspace = '\0';
    char *mnemonic = filecontents[i]; 
    char *argstr = firstspace + 1;
    trim(argstr);

    instruction_str_t *ins = getInstruction(instructions, mnemonic);
    
    // if command doesnt have any args throw an error
    if(ins == NULL || ins->buildInstruction == NULL) {
      printf("Error: instruction \"%s %s\" invalid or not implemented\n", 
          mnemonic, argstr);
      exit(EXIT_FAILURE);
    }
 
    int numargs;
    char **args;
    decoded_instruction_t *decoded;
    if(ins->type == BRANCH) {
      decoded = ins->buildInstruction(
          getSymbolAddressByName(table, argstr) - i - 2);
    } else {
      // count number of args
      char *original = argstr;
      for(numargs = 1; argstr[numargs]; 
          argstr[numargs] == ',' ? numargs++ : *argstr++);
      argstr = original;
  
      // split args from string to array
      args = calloc(numargs, sizeof(char*));
      char *arg = strtok(argstr, ",");
      for(int i = 0; i < numargs; i++) {
        args[i] = arg;
        trim(args[i]);
        arg = strtok(NULL, ",");
      }

      // create binary and free arguments
      decoded = ins->buildInstruction(args);
      free(args);
    }

    instruction_t binary = instruction_encode(decoded);
    free(decoded);

    // write to output in reverse order
    out[4*i] = binary;
    out[4*i+1] = binary >> 8;
    out[4*i+2] = binary >> 16;
    out[4*i+3] = binary >> 24;
    
  }
}

instruction_str_t *getInstruction(instruction_str_t *instructions, 
    char *mnemonic) {
  for(int i = 0; i < NUM_INSTRUCTIONS; i++) {
    if(strcmp(mnemonic, instructions[i].mnemonic) == 0) {
      return &instructions[i];
    }
  }
  return NULL;
}

void initInstructions(instruction_str_t *instructions) {
  
  // DATA PROCESS
  for(int i = 0; i <= 9; i++) instructions[i].type = DATA_PROCESS;
  instructions[0].mnemonic = "add";
  instructions[0].buildInstruction = &build_add;

  instructions[1].mnemonic = "sub";
  
  instructions[2].mnemonic = "rsb";
  
  instructions[3].mnemonic = "and";
  
  instructions[4].mnemonic = "eor";
  
  instructions[5].mnemonic = "orr";
  
  instructions[6].mnemonic = "mov";
  instructions[6].buildInstruction = &build_mov;

  instructions[7].mnemonic = "tst";
  
  instructions[8].mnemonic = "teq";
  
  instructions[9].mnemonic = "cmp";

  // MULTIPLICATION
  for(int i = 10; i <= 11; i++) instructions[i].type = MULTIPLY; 
  instructions[10].mnemonic = "mul";

  instructions[11].mnemonic = "mla";

  // SINGLE DATA TRANSFER
  for(int i = 12; i <= 13; i++) instructions[i].type = SINGLE_DATA_TRANSFER;
  instructions[12].mnemonic = "ldr";
  
  instructions[13].mnemonic = "str";
  
  // BRANCH
  for(int i = 14; i <= 20; i++) instructions[i].type = BRANCH;
  instructions[14].mnemonic = "beq";
  
  instructions[15].mnemonic = "bne";
  
  instructions[16].mnemonic = "bge";
  
  instructions[17].mnemonic = "blt";
  
  instructions[18].mnemonic = "bgt";
  
  instructions[19].mnemonic = "ble";
  
  instructions[20].mnemonic = "b";
 
  // SPECIAL
  for(int i = 21; i <= 22; i++) instructions[i].type = SPECIAL;
  instructions[21].mnemonic = "lsl";
  
  instructions[22].mnemonic = "andeq";
}
