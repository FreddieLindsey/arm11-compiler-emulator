#ifndef ASSEMBLE_H 
#define ASSEMBLE_H

/* typedefs // structs // enums */
struct symbol;
typedef struct symbolTableEntry symbol;
typedef enum error_code {
  ERROR_NONE, ERROR_USAGE, ERROR_INPUT_FILE_NOT_FOUND, ERROR_MALLOC,
  ERROR_CALLOC, ERROR_OUTPUT_FILE, ERROR_OUTPUT_FILE_WRITE
} error_code_t;

void freeTable(symbol *table);
int getSymbolAddressByName(symbol *table, char *name);
void printTable(symbol *table);
void printFileContents(char **filecontents);
void printBinary(unsigned char *binary);
char *getlabel(char *str);

/* Error handling */
void fail(error_code_t code);
void failif(int cond, error_code_t);

#endif
