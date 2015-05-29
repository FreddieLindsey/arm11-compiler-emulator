#ifndef FUNCPROTO
#define FUNCPROTO

/*
 *  TYPEDEFS / STRUCTS / UNIONS
 */

/*  GENERAL */
typedef uint32_t instruction_t;
typedef uint8_t memchunk_t;

/*  PIPELINE */
typedef struct pipeline {
  char** decoded;
  instruction_t fetched;
} pipeline_t;

/*  MACHINE */
typedef struct machine {
  memchunk_t *memory;
  uint32_t memsize;
  memchunk_t *registers;
  uint32_t regcount;
  pipeline_t *pipeline;
} machine_t;

/*
 *  FUNCTION PROTOTYPES
 */

/*  EMULATE */
void usage(void);

/*  FILEIO */
void loadfile(FILE *file, machine_t *machine);

/*  PIPELINE */
void init_pipeline(pipeline_t *pipeline);
void close_pipeline(pipeline_t *pipeline);
void run_pipeline(machine_t *machine);

/*  MACHINE */
void init_machine(machine_t *machine);
void close_machine(machine_t *machine);
void print_machine(machine_t *machine);

#endif
