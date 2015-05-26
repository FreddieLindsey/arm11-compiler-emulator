#ifndef ASSEMBLE_H 
#define ASSEMBLE_H

struct symbol;
typedef struct symbolTableEntry symbol;

void freeTable(symbol *table);
void printTable(symbol *table);
void printFileContents(char **filecontents);
void printBinary(unsigned char *binary);
void trimBefore(char *str);
void trimAfter(char *str);
char *getlabel(char *str);

#endif
