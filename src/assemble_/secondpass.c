#include "secondpass.h"

/*
 *  Performs the second pass on code written in [filecontents]
 *  [table] is a linked list of symbolTableEntry objects
 *  [filecontents] is an array of strings which should be modified to binary
 *    and written to [out]
 */
void secondpass(symbol *table, char **filecontents, unsigned char *out) {
  //dummy code, delete me
  out[0] = 0x01;
  out[1] = 0x10;
  out[2] = 0xa0;
  out[3] = 0xe3;

  out[4] = 0x02;
  out[5] = 0x20;
  out[6] = 0x81;
  out[7] = 0xe2;
}
