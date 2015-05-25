#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void loadfile(char *filename, int *memory);
char* byteToBinary(int x);

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("Usage: emulate <file>");
    return EXIT_FAILURE;
  }

  // Init memory and registers
  int *memory = calloc(65536, 1); // set 2^16 1 byte elements to 0
  //int *r[16] = { 0 }; // all registers init to 0

  // Load binary file to memory
  loadfile(argv[1], memory);

  return EXIT_SUCCESS;
}

/*
 *  Loads a binary file  
 */
void loadfile(char *filename, int *memory) {
  FILE *file = fopen(filename, "r");

  if(file == NULL) {
    printf("File \"%s\" not found\n", filename);
    return;
  }
  
  int x, i = 0;
  while((x = fgetc(file)) != EOF) {
    memory[i] = x;
    i++;
    // print memory as its loading in (debug)
    //printf("%s ", byteToBinary(x));
    //if(i % 4 == 0) printf("\n");
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
