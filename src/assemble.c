#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assemble_/assemble.h"
#include "assemble_/secondpass.h"

#define MAX_LINE_SIZE 512 * sizeof(char)
#define MEMORY_SIZE 65536

// symbol tables are stored in a linked list format
struct symbolTableEntry {
  char *name;
  int address;
  struct symbolTableEntry *next;
};

int main(int argc, char **argv) {

  if(argc != 3) {
    printf("Usage: assemble <input source> <output binary>\n");
    exit(EXIT_FAILURE);
  }

  // open file
  FILE *file = fopen(argv[1], "r");
  if(file == NULL) {
    printf("File \"%s\" not found\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // define symbol table
  symbol *symbolTable = malloc(sizeof(symbol));
  symbol *currentSymbol = symbolTable;

  // space to hold file contents
  char **filecontents = calloc(MEMORY_SIZE, sizeof(char*));

  // read file line by line
  int i = 0;
  char *line = (char*) malloc(MAX_LINE_SIZE);
  while(fgets(line, MAX_LINE_SIZE, file) != NULL) {

    // ignore empty lines 
    if(line[0] == '\n') continue;

    // remove leading whitespace
    trimBefore(line);

    // ignore lines that are only comments
    if(line[0] == ';') continue;
    
    // remove comments 
    line = strtok(line, ";");

    // remove ending whitespace
    trimAfter(line);

    // if line is a label, add it to the table
    char *label = getlabel(line);
    if(label != NULL) {
      // add this label to symbol table
      currentSymbol->name = strdup(label);
      currentSymbol->address = i; 
      currentSymbol->next = malloc(sizeof(symbol));
      currentSymbol = currentSymbol->next;
    } else {
      // copy line and add it to filecontents
      char* linedup = malloc(sizeof(line));
      linedup = strdup(line);
      filecontents[i] = linedup;
      i++;
    }

  }

  int numInstructions = i;

  free(line);
  fclose(file);
  
  //printf("Labels defined as:\n");
  //printTable(symbolTable);
  //printf("\nCode:\n");
  //printFileContents(filecontents);

  // allocate space for each 4 byte binary instruction
  unsigned char *binary = calloc(numInstructions*4, 1);
  secondpass(symbolTable, filecontents, binary);
  
  // free memory
  freeTable(symbolTable);
  free(filecontents);

  //printf("\nBinary:\n");
  //printBinary(binary);
 
  // write binary to file
  fwrite(binary, 1, numInstructions * 4, fopen(argv[2], "wb"));

  free(binary);
  return EXIT_SUCCESS;
}

/* 
 *  Basic string to int function
 */
int strtoi(char *str){
  return (int) strtol(str, NULL, 0);
}

/*
 *  Free all memory used by a symboltable
 */
void freeTable(symbol *table) {
  symbol *next, *current = table;
  while(current->next != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
}

/*
 *  Gets a symbol's address given its name
 */
int getSymbolAddressByName(symbol *table, char *name) {
  symbol *current = table;
  while(current->next != NULL) {
    if(strcmp(current->name, name) == 0) {
      return current->address; 
    }
    current = current->next;
  }
  printf("Error: symbol \"%s\" not found\n", name);
  exit(0);
}

/*
 *  Recursively prints a symbol table
 */
void printTable(symbol *table) {
  symbol *current = table;
  while(current->next != NULL) {
    printf("\"%s\" => %03d\n", current->name, current->address);
    current = current->next;
  }
}

void printFileContents(char **filecontents) {
  for(int i = 0; i < MEMORY_SIZE; i++) {
    if(filecontents[i] != 0) {
      printf("%03d: %s\n", i, filecontents[i]);
    }
  }
}

void printBinary(unsigned char *binary) {
  for(int i = 0; binary[i] != '\0'; i += 4) {
    printf("0x%02x%02x%02x%02x\n", 
        binary[i], binary[i+1], binary[i+2], binary[i+3]);
  }
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

/*
 *  Checks if [str] is a label, returns the label name if true, NULL otherwise
 */
char *getlabel(char *str) {

  // find first non-alphabetical character
  char* current = str;
  while(isalpha(*current)) {
    current++;
  }

  // if first non-alpha character is a :, end the line and return the name
  if(*current == ':') {
    *current = '\0';
    return str;
  } else {
    return NULL;
  }

}
