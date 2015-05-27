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
// 11100000 00000000 00000000 00000000
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
 *  TODO: operand2 as a register or shifted int
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
int operand2(char *str) {
  // if value is an immediate value
  if(str[0] == '#') {

    // convert to int, check for overflow
    int value = valueToInt(str);
    int shift = 0;
    // shift to fit in 8`bits
    while(value > (1 << 8) - 1) {
      value = value >> 1;
      shift++;
    }
    //1000 00111111 exptected
    //0001 01101100 actual
    const int SHIFT_OFFSET = 0x8;
    return value + (shift << SHIFT_OFFSET);
  } else if(str[0] == 'r') {
    return 0; //TODO: value is register
  } else {
    printf("Error: invalid operand2 \"%s\"\n", str);
    exit(EXIT_FAILURE);
  }
}

uint32_t mov(char *args) {
  
  // split args
  char *arg1 = strtok(args, ",");
  char *arg2 = strtok(NULL, ",");
  
  if(arg2 == NULL) {
    printf("Error: invalid arguments to \"mov %s\"\n", args); 
    exit(EXIT_FAILURE);
  }

  // remove whitespace
  trim(arg1);
  trim(arg2);

  // buld dataProcess structure
  dataProcess *ins = malloc(sizeof(dataProcess));
  ins->i = arg2[0] == '#' ? 1 : 0;
  ins->opcode = 13;
  ins->s = 0;
  ins->rn = 0;
  ins->rd = valueToInt(arg1);
  ins->operand2 = operand2(arg2);

  uint32_t result = buildDataProcess(ins);

  free(ins);

  return result;
}

