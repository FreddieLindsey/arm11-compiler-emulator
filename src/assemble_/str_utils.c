#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "str_utils.h"

/* 
 *  Basic string to int function
 */
int strtoi(char *str){
  return (int) strtol(str, NULL, 0);
}

/*
 *  Removes whitespace from the start and end of a string
 */
void trim(char *str) {
  trimBefore(str);
  trimAfter(str);
}

/*
 *  Removes whitespace from the start of a string
 */
void trimBefore(char *str) {
  
  // find first non-whitespace character
  char* newstart = str;
  while(*newstart != '\0' && isspace(*newstart)) {
    newstart++;
  }

  // remove everything before [newstart]
  memmove(str, newstart, strlen(newstart) + 1);
}

/*
 *  Removes whitespace from the end of a string
 */
void trimAfter(char *str) {
  
  // find last non-whitespace character
  char *newend = str + strlen(str) - 1;
  while(newend > str && isspace(*newend)) {
    newend--;
  }
  
  // end the string here 
  newend[1] = '\0';
}
