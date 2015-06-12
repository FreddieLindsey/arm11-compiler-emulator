#ifndef ASSEMBLE_H 
#define ASSEMBLE_H

/* typedefs // structs // enums */

// symbol tables are stored in a linked list format
typedef struct symbol_table_entry {
  char *name;
  int address;
  struct symbol_table_entry *next;
} symbol_t; 

// common error codes
typedef enum error_code {
  ERROR_NONE, ERROR_USAGE, ERROR_INPUT_FILE_NOT_FOUND, ERROR_MALLOC,
  ERROR_CALLOC, ERROR_OUTPUT_FILE, ERROR_OUTPUT_FILE_WRITE
} error_code_t;

typedef struct output_data {
  unsigned char *data;
  int num_instructions;
  int num_extra;
} output_data_t;

void free_table(symbol_t *table);
int get_address_by_symbol(symbol_t *table, char *name);
void print_table(symbol_t *table);
void print_file_contents(char **filecontents);
char *get_label(char *str);

/* Error handling */
void fail(error_code_t code);
void failif(int cond, error_code_t);

#endif
