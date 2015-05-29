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

    instruction *ins = getInstruction(instructions, mnemonic);
    
    // if command doesnt have any args throw an error
    if(ins == NULL || ins->createBinary == NULL) {
      printf("Error: instruction \"%s %s\" invalid or not implemented\n", 
          mnemonic, argstr);
      exit(EXIT_FAILURE);
    }
 
    int numargs;
    char **args;
    uint32_t binary;
    if(ins->type == BRANCH) {
      binary = ins->createBinary(getSymbolAddressByName(table, argstr) - i - 2);
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
      binary = ins->createBinary(args);
      free(args);
    }

    // write to output in reverse order
    out[4*i] = binary;
    out[4*i+1] = binary >> 8;
    out[4*i+2] = binary >> 16;
    out[4*i+3] = binary >> 24;
    
  }
}

instruction *getInstruction(instruction* instructions, char *mnemonic) {
  for(int i = 0; i < NUM_INSTRUCTIONS; i++) {
    if(strcmp(mnemonic, instructions[i].mnemonic) == 0) {
      return &instructions[i];
    }
  }
  return NULL;
}

void initInstructions(instruction* instructions) {
  
  // DATA PROCESS
  for(int i = 0; i <= 9; i++) instructions[i].type = DATA_PROCESS;
  instructions[0].mnemonic = "add";
  instructions[0].createBinary = &add;

  instructions[1].mnemonic = "sub";
  instructions[1].createBinary = &sub;
  
  instructions[2].mnemonic = "rsb";
  instructions[2].createBinary = &rsb;
  
  instructions[3].mnemonic = "and";
  instructions[3].createBinary = &and;
  
  instructions[4].mnemonic = "eor";
  instructions[4].createBinary = &eor;
  
  instructions[5].mnemonic = "orr";
  instructions[5].createBinary = &orr;
  
  instructions[6].mnemonic = "mov";
  instructions[6].createBinary = &mov;

  instructions[7].mnemonic = "tst";
  instructions[7].createBinary = &tst;
  
  instructions[8].mnemonic = "teq";
  instructions[8].createBinary = &teq;
  
  instructions[9].mnemonic = "cmp";
  instructions[9].createBinary = &cmp;

  // MULTIPLICATION
  for(int i = 10; i <= 11; i++) instructions[i].type = MULTIPLY; 
  instructions[10].mnemonic = "mul";
  instructions[10].createBinary = &mul; 

  instructions[11].mnemonic = "mla";
  instructions[11].createBinary = &mla;

  // SINGLE DATA TRANSFER
  for(int i = 12; i <= 13; i++) instructions[i].type = SINGLE_DATA_TRANSFER;
  instructions[12].mnemonic = "ldr";
  
  instructions[13].mnemonic = "str";
  
  // BRANCH
  for(int i = 14; i <= 20; i++) instructions[i].type = BRANCH;
  instructions[14].mnemonic = "beq";
  instructions[14].createBinary = &beq;
  
  instructions[15].mnemonic = "bne";
  
  instructions[16].mnemonic = "bge";
  
  instructions[17].mnemonic = "blt";
  
  instructions[18].mnemonic = "bgt";
  
  instructions[19].mnemonic = "ble";
  
  instructions[20].mnemonic = "b";
 
  // SPECIAL
  for(int i = 21; i <= 22; i++) instructions[i].type = SPECIAL;
  instructions[21].mnemonic = "lsl";
  instructions[21].createBinary = &lsl;
  
  instructions[22].mnemonic = "andeq";
  instructions[22].createBinary = &andeq;
}
