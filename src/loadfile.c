/*
 *  Test file to load a binary file
 */

#include <stdio.h>

void loadfile(char *filename) {
  printf("Loading file: %s \n", filename);
  FILE *file = fopen(filename, "r");

  if(file == NULL) {
    printf("File \"%s\" not found\n", filename);
    return;
  }
  
  printf("Printing file: \n");
  int x;
  while((x = fgetc(file)) != EOF) {
    printf("%c", x);
  }

  fclose(file);
}
