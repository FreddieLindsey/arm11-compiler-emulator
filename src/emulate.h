#ifndef FUNCPROTO
#define FUNCPROTO

#include "instructions.h"

/*
 *  TYPEDEFS / STRUCTS / UNIONS
 */

/*
 *  FUNCTION PROTOTYPES
 */

/*  EMULATE */
void usage(void);

/*  FILEIO */
void loadfile(FILE *file, machine_t *machine);

/*  PIPELINE */
void init_pipeline(pipeline_t *pipeline, machine_t *machine);
void close_pipeline(pipeline_t *pipeline);
void run_pipeline(machine_t *machine);

/*  MACHINE */
void init_machine(machine_t *machine);
void close_machine(machine_t *machine);
void print_machine(machine_t *machine);

/*  EXECUTE */


#endif
