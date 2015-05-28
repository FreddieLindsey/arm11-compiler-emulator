#include "secondpass.h"

/*
 *  Converts a register string, i.e. "r4" to the registers int value i.e. 4
 *  Also works with immediate numbers such as "#2"
 */
uint32_t valueToInt(char* str) {
  // increments str to remove first charcter, then converts to int
  return (uint32_t) strtol(++str, NULL, 0);
}


// DATA PROCESSING
//
// COND  I     S Rn   Rd 
// 111000XX XXXXXXXX XXXXXXXX XXXXXXXX
// OPCODE ^ ^^^          ^^^^ ^^^^^^^^
//                         OPERAND 2

typedef struct dataProcessIntruction {
  int i;
  int opcode;
  int s;
  int rn;
  int rd;
  int operand2;
} dataProcess;

/*
 *  Given a dataProcess, builds the binary and returns it
 */
uint32_t buildDataProcess(dataProcess *ins) {
  const int OFFSET_COND = 0x1C;
  const int OFFSET_I = 0x19;
  const int OFFSET_OPCODE = 0x15;
  const int OFFSET_S = 0x14;
  const int OFFSET_RN = 0x10;
  const int OFFSET_RD = 0xC;
  const int OFFSET_OPERAND2 = 0x0;
  int cond = 14;
  return (cond << OFFSET_COND) | (ins->i << OFFSET_I) | 
      (ins->opcode << OFFSET_OPCODE) | (ins->s << OFFSET_S) | 
      (ins->rn << OFFSET_RN) | (ins->rd << OFFSET_RD) | 
      (ins->operand2 << OFFSET_OPERAND2);
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
uint32_t computable(char **args, int opcode) {
  // create instruction object struct in heap
  dataProcess *ins = malloc(sizeof(dataProcess));
  ins->i = args[2][0] == '#' ? 1 : 0;
  ins->opcode = opcode;
  ins->s = 0;
  ins->rn = valueToInt(args[1]);
  ins->rd = valueToInt(args[0]);
  ins->operand2 = operand2(args[2], NULL);

  // calcualte binary for this instruction
  uint32_t result = buildDataProcess(ins);
  
  //free memory & return 
  free(ins);
  return result;
}

/*
 * Functions that set the flags
 */
uint32_t flagsetter(char **args, int opcode) {
  dataProcess *ins = malloc(sizeof(dataProcess));
  ins->i = args[1][0] == '#' ? 1 : 0;
  ins->opcode = opcode;
  ins->s = 1;
  ins->rn = valueToInt(args[0]);
  ins->rd = 0;
  ins->operand2 = operand2(args[1], args[2]);

  uint32_t result = buildDataProcess(ins);
  free(ins);
  return result;
}

uint32_t and(char **args) {
  return computable(args, 0);
}

uint32_t eor(char **args) {
  return computable(args, 1);
}

uint32_t sub(char **args) {
  return computable(args, 2);
}

uint32_t rsb(char **args) {
  return computable(args, 3);
}

uint32_t add(char **args) {
  return computable(args, 4);
}

uint32_t orr(char **args) {
  return computable(args, 12);
}

uint32_t mov(char **args) {
  dataProcess *ins = malloc(sizeof(dataProcess));
  ins->i = args[1][0] == '#' ? 1 : 0;
  ins->opcode = 13;
  ins->s = 0;
  ins->rn = 0;
  ins->rd = valueToInt(args[0]);
  ins->operand2 = operand2(args[1], args[2]);

  uint32_t result = buildDataProcess(ins);
  free(ins);
  return result;
}

uint32_t tst(char **args) {
  return flagsetter(args, 8);
}

uint32_t teq(char **args) {
  return flagsetter(args, 9);
}

uint32_t cmp(char **args) {
  return flagsetter(args, 10);
}

typedef struct multiplyInstruction {
  int a;
  int rd;
  int rn;
  int rs;
  int rm;
} multiply;

uint32_t buildMultiply(multiply *ins) {
  const int OFFSET_COND = 0x1C;
  const int OFFSET_A = 0x15;
  const int OFFSET_RD = 0x10;
  const int OFFSET_RN = 0xC;
  const int OFFSET_RS = 0x8;
  const int OFFSET_1001 = 0x4;
  const int OFFSET_RM = 0x0; 
  int cond = 14;
  return (cond << OFFSET_COND) | (ins->a << OFFSET_A) |
      (ins->rd << OFFSET_RD) | (ins->rn << OFFSET_RN) |
      (ins->rs << OFFSET_RS) | (0x9 << OFFSET_1001) |
      (ins->rm << OFFSET_RM);
}

uint32_t mla(char **args){
  multiply *ins = malloc(sizeof(multiply));
  ins->a = 1; 
  ins->rd = valueToInt(args[0]);
  ins->rn = valueToInt(args[3]);
  ins->rs = valueToInt(args[2]);
  ins->rm = valueToInt(args[1]); 

  uint32_t result = buildMultiply(ins);
  free(ins);
  return result;
  
}
