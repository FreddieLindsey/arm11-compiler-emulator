#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str_utils.h"
#include "secondpass.h"
#include "build_instruction.h"

#define NUM_INSTRUCTIONS 23

/*
 *    Performs the second pass on code written in [filecontents]
 *  [table] is a linked list of symbolTableEntry objects
 *  [filecontents] is an array of strings which should be converted to binary
 *  and written to [out], any extra binary used by ldr instructions in stored
 *  in [extradata]
 */
void secondpass(symbol *table, char **filecontents, output_data_t *out) {
  instruction_str_t *instructions = 
      calloc(NUM_INSTRUCTIONS, sizeof(instruction_str_t));
  failif(instructions == NULL, ERROR_CALLOC);
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
 
    decoded_instruction_t *decoded;
    if(ins->type == BRANCH) {
      decoded = ins->buildInstruction(
          getSymbolAddressByName(table, argstr) - i - 2);
    } else {

      int numargs;
      char **args;

      if(ins->type == DATA_PROCESS || ins->type == SINGLE_DATA_TRANSFER) {
        numargs = 4;
      } else {
        // count number of args
        char *original = argstr;
        for(numargs = 1; argstr[numargs]; 
            argstr[numargs] == ',' ? numargs++ : *argstr++);
        argstr = original;
      }
  
      // split args from string to array
      args = calloc(numargs, sizeof(char*));
      char *arg = strtok(argstr, ",");
      int j;
      for(j = 0; arg != NULL; j++) {
        args[j] = arg;
        trim(args[j]);
        arg = strtok(NULL, ",");
      }
      
      // create binary and free arguments
      if(ins->type == SINGLE_DATA_TRANSFER) {
        decoded = ins->buildInstruction(args, out, i);   
      } else {
        decoded = ins->buildInstruction(args);
      }

      free(args);
    }

    instruction_t binary = instruction_encode(decoded);
    free(decoded);

    // write to output in reverse order
    out->data[4*i] = binary;
    out->data[4*i+1] = binary >> 8;
    out->data[4*i+2] = binary >> 16;
    out->data[4*i+3] = binary >> 24;
    
  }

  free(instructions);
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
  instructions[1].buildInstruction = &build_sub;
  
  instructions[2].mnemonic = "rsb";
  instructions[2].buildInstruction = &build_rsb;
  
  instructions[3].mnemonic = "and";
  instructions[3].buildInstruction = &build_and;
  
  instructions[4].mnemonic = "eor";
  instructions[4].buildInstruction = &build_eor;
  
  instructions[5].mnemonic = "orr";
  instructions[5].buildInstruction = &build_orr;
  
  instructions[6].mnemonic = "mov";
  instructions[6].buildInstruction = &build_mov;

  instructions[7].mnemonic = "tst";
  instructions[7].buildInstruction = &build_tst;
  
  instructions[8].mnemonic = "teq";
  instructions[8].buildInstruction = &build_teq;
  
  instructions[9].mnemonic = "cmp";
  instructions[9].buildInstruction = &build_cmp;

  // MULTIPLICATION
  for(int i = 10; i <= 11; i++) instructions[i].type = MULTIPLY; 
  instructions[10].mnemonic = "mul";
  instructions[10].buildInstruction = &build_mul;

  instructions[11].mnemonic = "mla";
  instructions[11].buildInstruction = &build_mla;

  // SINGLE DATA TRANSFER
  for(int i = 12; i <= 13; i++) instructions[i].type = SINGLE_DATA_TRANSFER;
  instructions[12].mnemonic = "ldr";
  instructions[12].buildInstruction = &build_ldr;
  
  instructions[13].mnemonic = "str";
  instructions[13].buildInstruction = &build_str;
  
  // BRANCH
  for(int i = 14; i <= 20; i++) instructions[i].type = BRANCH;
  instructions[14].mnemonic = "beq";
  instructions[14].buildInstruction = &build_beq;
  
  instructions[15].mnemonic = "bne";
  instructions[15].buildInstruction = &build_bne;
  
  instructions[16].mnemonic = "bge";
  instructions[16].buildInstruction = &build_bge;
  
  instructions[17].mnemonic = "blt";
  instructions[17].buildInstruction = &build_blt;
  
  instructions[18].mnemonic = "bgt";
  instructions[18].buildInstruction = &build_bgt;
  
  instructions[19].mnemonic = "ble";
  instructions[19].buildInstruction = &build_ble;
  
  instructions[20].mnemonic = "b";
  instructions[20].buildInstruction = &build_b;
 
  // SPECIAL
  instructions[21].mnemonic = "lsl";
  instructions[21].type = LSL;
  instructions[21].buildInstruction = &build_lsl;
  
  instructions[22].mnemonic = "andeq";
  instructions[22].type = ANDEQ;
  instructions[22].buildInstruction = &build_andeq;
}
