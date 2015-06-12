#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "str_utils.h"

/*
 *  Checks if a line only contains whitespace characters
 *  Returns 0 if the string contains only whitespace, 1 otherwise;
 */
int is_empty(const char *str) {
  for(int i = 0; str[i]; i++) {
    if(!isspace(str[i])) return 0;
  }
  return 1;
}


/*
 *  Custom implementation of "strdup" as the library version was not supported
 *  on all systems
 */
char *strduplicate(const char *str) {
  int n = strlen(str) + 1;
  char *dup = malloc(n);
  if(dup) {
    strcpy(dup, str);
  }
  return dup;
}
/* 
 *  Basic string to int function
 */
int strtoi(char *str){
  return (int) strtol(str, NULL, 0);
}

/*
 *  Returns the last char in a string
 */
char last_char(char *str) {
  return str[strlen(str) - 1];
}

/*
 *  Removes whitespace from the start and end of a string
 */
void trim(char *str) {
  trim_before(str);
  trim_after(str);
}

/*
 *  Removes whitespace from the start of a string
 */
void trim_before(char *str) {
  
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
void trim_after(char *str) {
  
  // find last non-whitespace character
  char *newend = str + strlen(str) - 1;
  while(newend > str && isspace(*newend)) {
    newend--;
  }
  
  // end the string here 
  newend[1] = '\0';
}
