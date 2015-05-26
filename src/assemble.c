#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assemble_/secondpass.h"

#define MAX_LINE_SIZE 512 * sizeof(char)
#define MEMORY_SIZE 65536

// symbol tables are stored in a linked list format
struct symbolTableEntry {
  char *name;
  int address;
  struct symbolTableEntry *next;
};

typedef struct symbolTableEntry symbol;

void printTable(symbol *table);
void printFileContents(char **filecontents);
void trimBefore(char *str);
void trimAfter(char *str);
char *getlabel(char *str);

int main(int argc, char **argv) {

  if(argc != 2) {
    printf("Usage: assemble <file>\n");
    return EXIT_FAILURE;
  }

  // open file
  FILE *file = fopen(argv[1], "r");
  if(file == NULL) {
    printf("File \"%s\" not found\n", argv[1]);
    return EXIT_FAILURE;
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

  free(line);
  
  printf("Labels defined as:\n");
  printTable(symbolTable);
  printf("\nCode:\n");
  printFileContents(filecontents);

  fclose(file);

  //second pass convert to binary
  //write binary file
 
  //TODO: free line
  //TODO: free table
  free(filecontents);
  return EXIT_SUCCESS;
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
