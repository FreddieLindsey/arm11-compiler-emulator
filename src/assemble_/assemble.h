#ifndef ASSEMBLE_H 
#define ASSEMBLE_H

struct symbol;
typedef struct symbolTableEntry symbol;

int strtoi(char *str);
void freeTable(symbol *table);
int getSymbolAddressByName(symbol *table, char *name);
void printTable(symbol *table);
void printFileContents(char **filecontents);
void printBinary(unsigned char *binary);
void trim(char *str);
void trimBefore(char *str);
void trimAfter(char *str);
char *getlabel(char *str);

#endif
