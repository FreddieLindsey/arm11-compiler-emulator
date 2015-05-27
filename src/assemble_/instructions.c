#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "secondpass.h"

/*
 *  Converts a register string, i.e. "r4" to the registers int value i.e. 4
 *  Also works with immediate numbers such as "#2"
 */
int valueToInt(char* str) {
  // increments str to remove first charcter, then converts to int
  return (int) strtol(++str, NULL, 0);
}


// DATA PROCESSING
//                        
//           Rd         S Rn  COND  I
// 00000000 00000000 00000000 11100000
// ^^^^^^^^     ^^^^ ^^^             ^
//    OPERAND 2            OPCODE

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
int buildDataProcess(dataProcess *ins) {
  int cond = 14;
  return (cond << 4) | (ins->i << 1) | (ins->opcode & (1 << 3) >> 3) | 
      ((ins->opcode & 7) << 13) | (ins->s << 12) | (ins->rn << 8) |
      (ins->rd << 20) | (ins->operand2 << 24);
}

int mov(char *args) {
  // split args and remove whitespace
  char *arg1 = strtok(args, ",");
  char *arg2 = strtok(NULL, ",");
  trim(arg1);
  trim(arg2);

  dataProcess *ins = malloc(sizeof(dataProcess));
  ins->i = arg2[0] == '#' ? 1 : 0;
  ins->opcode = 13;
  ins->s = 0;
  ins->rn = 0;
  ins->rd = valueToInt(arg1);
  ins->operand2 = valueToInt(arg2);

  return buildDataProcess(ins);
}

