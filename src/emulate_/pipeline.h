#ifndef PIPELINE_H
#define PIPELINE_H

#include "machine/machine.h"

/*
 *  TYPEDEFS / STRUCTS / UNIONS
 */

typedef struct pipeline {
  char** decoded;
  instruction_t fetched;
} pipeline_t;

/*
 *  FUNCTION PROTOTYPES
 */

void init_pipeline(pipeline_t *pipeline);
void close_pipeline(pipeline_t *pipeline);
void run_pipeline(pipeline_t *pipeline, machine_t *machine);
void execute(pipeline_t *pipeline, machine_t *machine);
void decode(pipeline_t *pipeline, machine_t *machine);
void fetch(pipeline_t *pipeline, machine_t *machine);

#endif
