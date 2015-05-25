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
 *  Loads a binary file  
 */
void loadfile(char *filename) {
  FILE *file = fopen(filename, "r");

  if(file == NULL) {
    printf("File \"%s\" not found\n", filename);
    return;
  }
  
  int i = 0;
  int x;
  while((x = fgetc(file)) != EOF) {
    printf("%s ", byteToBinary(x));
    i++;
    if(i % 4 == 0) printf("\n");
  }

  fclose(file);
}

/*
 *  Converts a byte to a binary string
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
