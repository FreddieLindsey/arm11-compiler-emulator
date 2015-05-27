#ifndef MACHINE
#define MACHINE

/*
 *  TYPEDEFS / STRUCTS / UNIONS
 */

typedef uint32_t instruction_t;
typedef uint8_t memchunk_t;

typedef struct machine {
  memchunk_t *memory;
  uint32_t memsize;
  memchunk_t *registers;
  uint32_t regcount;
} machine_t;

#endif
