#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void loadfile(char *filename);
char* byteToBinary(int x);

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("Usage: emulate <file>");
    return EXIT_FAILURE;
  }
  loadfile(argv[1]);
  return EXIT_SUCCESS;
}

/*
 *  Loads a binary file and prints it out
 *  TODO: make it return a data structure with the binary commands rahter than
 *        just printing it out
 */
void loadfile(char *filename) {
  FILE *outStream = stdout;

  fprintf(outStream, "Loading file: %s \n", filename);
  FILE *file = fopen(filename, "r");

  if(file == NULL) {
    fprintf(outStream, "File \"%s\" not found\n", filename);
    return;
  }
  
  fprintf(outStream, "Printing file: \n");
  int x;
  while((x = fgetc(file)) != EOF) {
    printf("%s", byteToBinary(x));
  }

  fclose(file);
}

/*
 *  Converts a byte to its binary representation
 *  Helper function to print out a byte for debugging
 */
char* byteToBinary(int x) {
  static char buffer[9];
  buffer[0] = '\0';

  for(int i = 128; i > 0; i >>= 1) {
    strcat(buffer, ((x & i) == i) ? "1" : "0");
  }

  return buffer;
}
