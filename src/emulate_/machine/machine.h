#ifndef MACHINE
#define MACHINE

/*
** The structure for the ARM machine
*/

typedef struct machine {
  char *memory;
  int *r; // General purpose registers
  int pc; // Other registers
  int cpsr;
} machine_t;

#endif
