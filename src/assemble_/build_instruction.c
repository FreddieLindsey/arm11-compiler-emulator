#include "../instructions.h"
#include "assemble.h"
#include "build_instruction.h"
#include "str_utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ARG_SIZE 512 * sizeof(char)
#define PC_ADDRESS 15

/*
 *  Converts a register string, i.e. "r4" to the registers int value i.e. 4
 *  Also works with immediate numbers such as "#2" and values with square 
 *  brackets e.g. "[r3]"
 */
int valueToInt(char* str) {
  char *copy = strduplicate(str);
  char *copyOrig = copy;

  // remove starting [ and ending ]
  if(copy[0] == '[') copy++;
  if(lastChar(copy) == ']') copy[strlen(copy) - 1] = '\0'; 

  // check for PC
  if(strcmp(copy, "PC") == 0) return PC_ADDRESS;

  // remove first char if its not numeric
  if(!isdigit(copy[0])) copy++;

  int result = strtoi(copy);
  free(copyOrig);
  return result;
}

uint16_t build_shift(char *shiftstr) {
  
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
      (isregistershift << IS_REG_OFFSET);
}

/* DATA TRANSFER */

/*
 *  Given a [str] calculate the operand2 binary
 */
uint16_t build_operand2(char *str, char *shiftstr) {

  instruction_t value = valueToInt(str);
  
  // if value is an immediate value
  if(str[0] == '#' || str[0] == '=') {
    
    // shift to fit in 8`bits
    int shift = 0;

    // while value does not fit in 8 bits
    while(value > (1 << 8) - 1) {
      // rotate twice
      int mask = 3 << 30;
      int excess = (value & mask) >> 30;
      value = (value << 2) | excess;
      shift++;
    }

    const int SHIFT_OFFSET = 0x8;
    return value + (shift << SHIFT_OFFSET);
  } else if(str[0] == 'r') {

    // if value needs to be shifted
    if(shiftstr != NULL) {
      return build_shift(shiftstr) | value; 

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
  failif(ins == NULL, ERROR_MALLOC);
  ins->kind = DATA_PROCESS;
  ins->opcode = opcode;
  ins->immediate = args[2][0] == '#' ? 1 : 0;
  ins->set = 0;
  ins->regn = valueToInt(args[1]);
  ins->regd = valueToInt(args[0]);
  ins->operand2 = build_operand2(args[2], args[3]); 

  return ins;
}

/*
 * Functions that set the flags
 */
decoded_instruction_t *flagsetter(char **args, int opcode) {
  decoded_instruction_t *ins = malloc(sizeof(decoded_instruction_t));
  ins->kind = DATA_PROCESS;
  ins->opcode = opcode;
  ins->immediate = args[1][0] == '#' ? 1 : 0;
  ins->set = 1;
  ins->regn = valueToInt(args[0]);
  ins->regd = 0;
  ins->operand2 = build_operand2(args[1], args[2]);
  return ins;
}

decoded_instruction_t *build_add(char **args) { return computable(args, 4); }
decoded_instruction_t *build_sub(char **args) { return computable(args, 2); }
decoded_instruction_t *build_rsb(char **args) { return computable(args, 3); } 
decoded_instruction_t *build_and(char **args) { return computable(args, 0); }
decoded_instruction_t *build_eor(char **args) { return computable(args, 1); }
decoded_instruction_t *build_orr(char **args) { return computable(args, 12); }
decoded_instruction_t *build_tst(char **args) { return flagsetter(args, 8); }
decoded_instruction_t *build_teq(char **args) { return flagsetter(args, 9); }
decoded_instruction_t *build_cmp(char **args) { return flagsetter(args, 10); }

decoded_instruction_t *build_mov(char **args) {
  decoded_instruction_t *ins = malloc(sizeof(decoded_instruction_t));
  ins->kind = DATA_PROCESS;
  ins->opcode = 13;
  ins->immediate = args[1][0] == '#' || args[1][0] == '=' ? 1 : 0;
  ins->set = 0;
  ins->regn = 0;
  ins->regd = valueToInt(args[0]);
  ins->operand2 = build_operand2(args[1], args[2]);
  return ins;
}

/* MULTIPLY */

decoded_instruction_t *build_mul(char **args) {
  decoded_instruction_t *ins = malloc(sizeof(decoded_instruction_t));
  ins->kind = MULTIPLY;
  ins->accumulate = 0;
  ins->regd = valueToInt(args[0]);
  ins->regs = valueToInt(args[2]);
  ins->regm = valueToInt(args[1]);
  ins->regn = 0;
  return ins;
}

decoded_instruction_t *build_mla(char **args) {
  decoded_instruction_t *ins = malloc(sizeof(decoded_instruction_t));
  ins->kind = MULTIPLY;
  ins->accumulate = 1;
  ins->regd = valueToInt(args[0]);
  ins->regn = valueToInt(args[3]);
  ins->regs = valueToInt(args[2]);
  ins->regm = valueToInt(args[1]);
  return ins;
}

/* SINGLE DATA TRANSFER */

decoded_instruction_t *build_sdt(char **args, int loadstore) {
  decoded_instruction_t *ins = malloc(sizeof(decoded_instruction_t));  
  ins->kind = SINGLE_DATA_TRANSFER;
  
  // if arg 2 does not exist, offset is 0
  if(args[2] == NULL) {
    ins->offset = 0;
    ins->immediate = 0;
    ins->up = 1;
    ins->prepost = 1;
  } else {
    // prepost set if last char of 2nd arg is not a ']' 
    ins->prepost = lastChar(args[1]) == ']' ? 0 : 1;

    if(args[3] == NULL) {
      int offset = valueToInt(args[2]); 

      // if offset is a register
      if(args[2][0] == 'r') {
        ins->immediate = 1;
        ins->offset = offset;
        ins->up = 1;
      } else {

        // check for negative offset
        if(offset < 0) {
          ins->offset = -1 * offset;
          ins->up = 0;
        } else {
          ins->offset = offset;
          ins->up = 1;
        }
        ins->immediate = 0; 
      }
    } else {
      //if theres a shift
      ins->offset = build_operand2(args[2], args[3]);
      ins->immediate = 1;
      ins->up = 1;
    }
  }

  ins->loadstore = loadstore;
  ins->regn = valueToInt(args[1]);
  ins->regd = valueToInt(args[0]);

  return ins;
}

decoded_instruction_t *build_ldr(char **args, output_data_t *out, int pos) {

  decoded_instruction_t *ins;
  
  // constant values
  if(args[1][0] == '=') {
    int value = valueToInt(args[1]);
    
    // if value fits in mov instruction
    if(value <= 0xFF) {
      return build_mov(args);
    } else {
      // load extra data into the end of the binary file
      out->data[(out->numInstructions + out->numExtra) * 4] = value;
      out->data[(out->numInstructions + out->numExtra) * 4 + 1] = value >> 8;
      out->data[(out->numInstructions + out->numExtra) * 4 + 2] = value >> 16;
      out->data[(out->numInstructions + out->numExtra) * 4 + 3] = value >> 24;

      // calculate offset 
      int offset = (out->numInstructions - pos + out->numExtra) * 4 - 8;
      out->numExtra++;

      // build new arguments + recurse
      char *newarg1 = malloc(MAX_ARG_SIZE * sizeof(char));
      char *newarg2 = malloc(MAX_ARG_SIZE * sizeof(char));
      sprintf(newarg1, "[PC");
      sprintf(newarg2, "%d]", offset);
      args[1] = newarg1; 
      args[2] = newarg2; 
      decoded_instruction_t *result = build_ldr(args, out, pos);
      free(newarg1);
      free(newarg2);
      return result;
    }
  } else if(args[1][0] == '[') {
    return build_sdt(args, 1); 
  } else {
    printf("Error: invalid argument to ldr \"%s\"\n", args[1]);
    exit(EXIT_FAILURE);
  }

  return ins;
}

decoded_instruction_t *build_str(char **args, output_data_t *out, int pos) {
  return build_sdt(args, 0);
}

/* BRANCH */
  
decoded_instruction_t *branch(int offset, int opcode) {
  decoded_instruction_t *ins = malloc(sizeof(decoded_instruction_t));
  ins->kind = BRANCH;
  ins->cond = opcode;
  ins->offset = offset;
  return ins;
}

decoded_instruction_t *build_beq(int offset) { return branch(offset, 0); }
decoded_instruction_t *build_bne(int offset) { return branch(offset, 1); }
decoded_instruction_t *build_bge(int offset) { return branch(offset, 10); }
decoded_instruction_t *build_blt(int offset) { return branch(offset, 11); }
decoded_instruction_t *build_bgt(int offset) { return branch(offset, 12); }
decoded_instruction_t *build_ble(int offset) { return branch(offset, 13); }
decoded_instruction_t *build_b(int offset) { return branch(offset, 14); }

/* SPECIAL */

decoded_instruction_t *build_andeq(void) {
  decoded_instruction_t *ins = malloc(sizeof(decoded_instruction_t));
  ins->kind = ANDEQ;
  return ins;
}

decoded_instruction_t *build_lsl(char **args) {
  
  // create space for 4 arguments to simulate mov instruction
  char **movargs = malloc(3 * sizeof(char*));
  failif(movargs == NULL, ERROR_MALLOC);
  movargs[0] = args[0];
  movargs[1] = args[0];
  char *arg3 = malloc(MAX_ARG_SIZE);
  memset(arg3, 0, MAX_ARG_SIZE);
  memcpy(arg3, "lsl ", 4*sizeof(char));
  movargs[2] = strcat(arg3, args[1]);

  decoded_instruction_t *result = build_mov(movargs);
  
  free(arg3);
  free(movargs);

  return result;
}
