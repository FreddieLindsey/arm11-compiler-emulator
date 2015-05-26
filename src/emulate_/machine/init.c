/*
 * Fetches a single instruction
 * given a starting address
 */
int fetchInstruction(int *x) {
  int instruction = 0;
  //for(int i = 0; i < 3; i++) {
  //  instruction += (int) byteToBinary(*x);
  //  instruction = instruction * 100000000;
  //  x++;
  //}
  //instruction += (int) byteToBinary(*x);
  return instruction;
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



/*
 *  Loads a binary file to [*memory]
 */
void loadfile(char *filename, char *memory) {
  FILE *file = fopen(filename, "r");

  if (file == NULL) {
    printf("File \"%s\" not found\n", filename);
    return;
  }

  char x;
  int i = 0;
  while((x = fgetc(file)) != EOF) {
    memory[i] = x;
    i++;
    //print memory as its loading in (debug)
    //printf("%s ", byteToBinary(x));
    //if(i % 4 == 0) printf("\n");
  }

  fclose(file);
}