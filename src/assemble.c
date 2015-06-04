#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assemble_/str_utils.h"
#include "assemble_/assemble.h"
#include "assemble_/secondpass.h"

#define MAX_ERROR_STRING_SIZE 128 * sizeof(char)
#define MAX_LINE_SIZE 512 * sizeof(char)
#define MEMORY_SIZE 65536

// symbol tables are stored in a linked list format
struct symbolTableEntry {
  char *name;
  int address;
  struct symbolTableEntry *next;
};

int main(int argc, char **argv) {

  // check correct number of arguments
  failif(argc != 3, ERROR_USAGE);

  // open file
  FILE *file = fopen(argv[1], "r");
  failif(file == NULL, ERROR_INPUT_FILE_NOT_FOUND);

  // define symbol table
  symbol *symbolTable = malloc(sizeof(symbol));
  failif(symbolTable == NULL, ERROR_MALLOC);
  symbol *currentSymbol = symbolTable;

  // space to hold file contents
  char **filecontents = calloc(MEMORY_SIZE, sizeof(char*));
  failif(filecontents == NULL, ERROR_CALLOC);

  // read file line by line
  int i = 0;
  char *line = (char*) malloc(MAX_LINE_SIZE);
  failif(line == NULL, ERROR_MALLOC);

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
      failif(linedup == NULL, ERROR_MALLOC);
      linedup = strdup(line);
      filecontents[i] = linedup;
      i++;
    }

  }

  int numInstructions = i;

  free(line);
  fclose(file);
  
  // allocate space for each 4 byte binary instruction
  output_data_t *output = malloc(sizeof(output_data_t));
  output->data = calloc(numInstructions * 4 * 2, 1);
  output->numInstructions = numInstructions;
  output->numExtra = 0;
  failif(output == NULL, ERROR_MALLOC);

  // run secondpass
  secondpass(symbolTable, filecontents, output);
  
  // free memory
  freeTable(symbolTable);
  free(filecontents);

  // write binary to file
  FILE *out = fopen(argv[2], "wb");
  failif(out == NULL, ERROR_OUTPUT_FILE);

  size_t writesize = fwrite(output->data, 1, 
     (numInstructions + output->numExtra) * 4, fopen(argv[2], "wb"));
  failif(writesize == 0, ERROR_OUTPUT_FILE_WRITE);

  free(output->data);
  free(output);
  return EXIT_SUCCESS;
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
  fail(ERROR_NONE);
  return 0;
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
 *  Checks if [str] is a label, returns the label name if true, NULL otherwise
 */
char *getlabel(char *str) {

  // check first char is alphetical
  if(!isalpha(str[0])) return NULL;

  // find first non-alphanumeric character
  char* current = str;
  while(isalnum(*current)) {
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

/*
 *  Throws an error given an error code [code]
 */
void fail(error_code_t code) {
  char *error_string = malloc(MAX_ERROR_STRING_SIZE);
  switch(code) {
    case ERROR_NONE: break;
    case ERROR_USAGE:
      error_string = "Usage: assemble <input source> <output binary>";
      break;
    case ERROR_INPUT_FILE_NOT_FOUND:
      error_string = "Input file not found";
      break;
    case ERROR_MALLOC:
      error_string = "Malloc";
      break;
    case ERROR_CALLOC:
      error_string = "Calloc";
      break;
    case ERROR_OUTPUT_FILE:
      error_string = "Loading output file failed, check directory exists";
      break;
    case ERROR_OUTPUT_FILE_WRITE:
      error_string = "Writing to output file failed";
      break;
    default:
      error_string = "Unknown";
      break;
  }
  fprintf(stdout, "Error: %s\n", error_string);
  exit(EXIT_FAILURE);
}

/*
 *  Throws an error if [cond] is false
 */
void failif(int cond, error_code_t code) {
  if(cond) fail(code);
}

