#include "../instructions.h"
#include "build_instruction.h"
#include "assemble.h"
#include <stdlib.h>
#include <string.h>

/* 
 *  Basic string to int function
 */
int strtoi(char *str){
  return (int) strtol(str, NULL, 0);
}

/*
 *  Converts a register string, i.e. "r4" to the registers int value i.e. 4
 *  Also works with immediate numbers such as "#2"
 */
int valueToInt(char* str) {
  // increments str to remove first charcter, then converts to int
  return strtoi(++str);
}

/*
 *  Given a [str] calculate the operand2 binary
 */
int operand2(char *str, char *shiftstr) {
  
  // if value is an immediate value
  if(str[0] == '#') {
    
    // convert to int, check for overflow
    uint32_t value = valueToInt(str);
    int shift = 0;
    
    // shift to fit in 8`bits
    while(value > (1 << 8) - 1) {
      // rotate twice
      int excess = (value & (11 << 30)) >> 30;
      value = (value << 2) | excess;
      shift++;
    }

    const int SHIFT_OFFSET = 0x8;
    return value + (shift << SHIFT_OFFSET);
  } else if(str[0] == 'r') {
    
    // get register number
    int value = valueToInt(str);

    // if value needs to be shifted
    if(shiftstr != NULL) {
      
      // split by space into shift type and amount
      char *firstspace = strchr(shiftstr, ' ');
      if(firstspace == NULL) {
        printf("Error: shift string \"%s\" invalid\n", shiftstr);
        exit(EXIT_FAILURE);
      } 
      *firstspace = '\0';
      char *shifttype = shiftstr; 
      char *shiftamount = firstspace + 1;
      trim(shifttype);
      trim(shiftamount);

      // work out shift amount based on either direct value or register
      int shiftamountvalue = valueToInt(shiftamount);
      int amount;
      int isregistershift;
      if(shiftamount[0] == '#') {
        amount = shiftamountvalue; 
        isregistershift = 0;
      } else if(shiftamount[0] == 'r') {
        amount = shiftamountvalue << 1; 
        isregistershift = 1;
      } else {
        printf("Error: shift amount \"%s\" invalid\n", shiftamount);
        exit(EXIT_FAILURE);
      }
      
      // switch on shift type & find shift type binary
      int type;
      if(strcmp(shifttype, "asr") == 0) {
        type = 2;
      } else if(strcmp(shifttype, "lsl") == 0) {
        type = 0;
      } else if(strcmp(shifttype, "lsr") == 0) {
        type = 1;
      } else if(strcmp(shifttype, "ror") == 0) {
        type = 3;
      } else {
        printf("Error: shift type \"%s\" invalid\n", shifttype);
        exit(EXIT_FAILURE);
      }

      const int AMOUNT_OFFSET = 0x7; 
      const int TYPE_OFFSET = 0x5;
      const int IS_REG_OFFSET = 0x4; 
      return (amount << AMOUNT_OFFSET) | (type << TYPE_OFFSET) | 
          (isregistershift << IS_REG_OFFSET) | value;

    } else {
      return value;
    }
  } else {
    printf("Error: invalid argument \"%s\"\n", str);
    exit(EXIT_FAILURE);
  }
}

/*
 *  Functions that compute results (and, add, sub, etc..)
 */ 
decoded_instruction_t *computable(char **args, int opcode) {
  // create instruction object struct in heap
  decoded_instruction_t *ins = malloc(sizeof(decoded_instruction_t));
  ins->kind = DATA_PROCESS;
  ins->opcode = opcode;
  ins->immediate = args[2][0] == '#' ? 1 : 0;
  ins->set = 0;
  ins->regn = valueToInt(args[1]);
  ins->regd = valueToInt(args[0]);
  ins->operand2 = operand2(args[2], NULL);
  
  return ins;
}

decoded_instruction_t *build_add(char **args) {
  return computable(args, 4);
}

decoded_instruction_t *build_mov(char **args) {
  decoded_instruction_t *ins = malloc(sizeof(decoded_instruction_t));
  ins->kind = DATA_PROCESS;
  ins->opcode = 13;
  ins->immediate = args[1][0] == '#' ? 1 : 0;
  ins->set = 0;
  ins->regn = 0;
  ins->regd = valueToInt(args[0]);
  ins->operand2 = operand2(args[1], args[2]);

  return ins;
}
