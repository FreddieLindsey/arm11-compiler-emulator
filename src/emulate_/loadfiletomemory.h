void loadfile(char *filename, char *memory);

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
