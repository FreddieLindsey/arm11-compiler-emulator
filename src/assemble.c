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


int main(int argc, char **argv) {

  // check correct number of arguments
  failif(argc != 3, ERROR_USAGE);

  // open file
  FILE *file = fopen(argv[1], "r");
  failif(file == NULL, ERROR_INPUT_FILE_NOT_FOUND);

  // define symbol_t table
  symbol_t *symbolTable = malloc(sizeof(symbol_t));
  memset(symbolTable, 0, sizeof(symbol_t));
  failif(symbolTable == NULL, ERROR_MALLOC);
  symbol_t *currentSymbol = symbolTable;

  // space to hold file contents
  char **filecontents = calloc(MEMORY_SIZE, sizeof(char*));
  failif(filecontents == NULL, ERROR_CALLOC);

  // read file line by line
  int i = 0;
  char *line = (char*) malloc(MAX_LINE_SIZE);
  failif(line == NULL, ERROR_MALLOC);

  while(fgets(line, MAX_LINE_SIZE, file) != NULL) {
    // ignore empty lines 
    if(is_empty(line)) continue;

    // remove leading whitespace
    trim_before(line);

    // ignore lines that are only comments
    if(line[0] == ';') continue;
    
    // remove comments 
    line = strtok(line, ";");

    // remove ending whitespace
    trim_after(line);

    // if line is a label, add it to the table
    char *label = get_label(line);
    if(label != NULL) {
      // add this label to symbol_t table
      currentSymbol->name = strduplicate(label);
      currentSymbol->address = i; 
      currentSymbol->next = malloc(sizeof(symbol_t));
      memset(currentSymbol->next, 0, sizeof(symbol_t));
      currentSymbol = currentSymbol->next;
    } else {
      // copy line and add it to filecontents
      char *linedup = strduplicate(line);
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
  output->num_instructions = numInstructions;
  output->num_extra = 0;
  failif(output == NULL, ERROR_MALLOC);

  // run secondpass
  secondpass(symbolTable, filecontents, output);
  
  // free memory
  free_table(symbolTable);
  for(int i = 0; i < MEMORY_SIZE; i++) {
    free(filecontents[i]);
  }
  free(filecontents);

  // write binary to file
  FILE *out = fopen(argv[2], "wb");
  failif(out == NULL, ERROR_OUTPUT_FILE);

  size_t writesize = fwrite(output->data, 1, 
     (numInstructions + output->num_extra) * 4, fopen(argv[2], "wb"));
  failif(writesize == 0, ERROR_OUTPUT_FILE_WRITE);

  free(output->data);
  free(output);
  return EXIT_SUCCESS;
}


/*
 *  Free all memory used by a symboltable
 */
void free_table(symbol_t *table) {
  symbol_t *next, *current = table;
  while(current != NULL) {
    next = current->next;
    free(current->name);
    free(current);
    current = next;
  }
}

/*
 *  Gets a symbol's address given its name
 */
int get_address_by_symbol(symbol_t *table, char *name) {
  symbol_t *current = table;
  while(current->next != NULL) {
    if(strcmp(current->name, name) == 0) {
      return current->address; 
    }
    current = current->next;
  }
  printf("Error: symbol_t \"%s\" not found\n", name);
  fail(ERROR_NONE);
  return 0;
}

/*
 *  Recursively prints a symbol_t table
 */
void print_table(symbol_t *table) {
  symbol_t *current = table;
  while(current->next != NULL) {
    printf("\"%s\" => %03d\n", current->name, current->address);
    current = current->next;
  }
}

void print_file_contents(char **filecontents) {
  for(int i = 0; i < MEMORY_SIZE; i++) {
    if(filecontents[i] != 0) {
      printf("%03d: %s\n", i, filecontents[i]);
    }
  }
}

/*
 *  Checks if [str] is a label, returns the label name if true, NULL otherwise
 */
char *get_label(char *str) {

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

