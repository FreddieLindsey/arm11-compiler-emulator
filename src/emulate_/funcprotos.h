#ifndef FUNCPROTO
#define FUNCPROTO

#include "machine/machine.h"

/*
 *  FUNCTION PROTOTYPES
 */

void usage(void);
void loadfile(FILE *file, machine_t *machine);
void init_machine(machine_t *machine);
void close_machine(machine_t *machine);
void print_machine(machine_t *machine);

#endif
