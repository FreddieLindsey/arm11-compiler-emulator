#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <libgen.h>
#include <string.h>

/*
 *  STRUCTS / UNIONS
 */ 

typedef struct machine {
  uint32_t *memory;
  uint32_t *registers; 
} machine_t;

/*
 *  FUNCTION PROTOTYPES
 */

void usage(void);
void init_machine(machine_t *machine, int membytes, int regs);
void printmachine(machine_t *machine);
void close_machine(machine_t *machine);
void loadfile(FILE *file, machine_t *machine);

/*
 *  MAIN
 */

int main(int argc, char** argv) {

  /*  Constant to enable verbose mode */
  static int verbose = 0;

  /*  Check if a file has been given as an argument
      return if not */
  if (argc != 2 && !(argc > 2 && (strcmp(argv[2], "v") == 0))) {
    usage();
    printf("You have supplied %s arguments.\nEXITING\n", 
          (argc < 2) ? "too few" : "too many" );
    exit(EXIT_FAILURE);
  } else if (argc > 2 && (strcmp(argv[2], "v") == 0 || strcmp(argv[2],"V") == 0)) {
    verbose = 1;
  }

  /*  Check a given filename exists, and return if not */ 
  FILE *file;
  if ((file = fopen(argv[1], "r")) == NULL) {
    perror("Error opening file!");
    printf("Does the file %s exist?\nEXITING\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  if (verbose) printf("File opened successfully:\t%s\n", basename(argv[1]));

  /*  Create and initialise a machine */
  machine_t machine;
  init_machine(&machine, 65536, 16);  
  if (verbose) printf("Machine initialised.\n");

  /*  Load a file into machine memory, then close it */
  loadfile(file, &machine);

  /*  Print the state of the machine */
  if (verbose) printmachine(&machine);

  /*  Frees the machine by closing it */
  close_machine(&machine);
  if (verbose) printf("Machine closed, memory freed\n");
  
  return EXIT_SUCCESS;

}

/*
 *  FUNCTIONS
 */

void usage(void) {
  printf( "Program takes 1 argument:\n\nExample:\t"
          "./emulate binary_arm_file\n\n");
}

void init_machine(machine_t *machine, int membytes, int regs) {
  machine->memory = calloc(membytes, sizeof(char));
  machine->registers = calloc(regs, sizeof(int));
}

void printmachine(machine_t *machine) {
  /* Signal start of machine to user */
  printf("\nPrinting machine state:\n");

  /* Print register values */
  printf("\nRegister values:\n");
  for (int i = 0; i < 16; ++i) {
    printf("Register No. %2i:\t0x%08x\n", i + 1, 
           machine->registers[i]);
  }

  /* Print non-zero memory */
  printf("\nNon-zero memory:\n");
  for (int i = 0; i < 16384; ++i) {
    if (machine->memory[i] != 0) printf("Memory Location 0x%08x:\t0x%08x\n", i * 4, machine->memory[i]);
  }

  /* Signal end of machine to user */
  printf("\nEnd of machine state.\n");
}

void close_machine(machine_t *machine) {
  free(machine->memory);
  free(machine->registers);
}

void loadfile(FILE *file, machine_t *machine) {
  /* Read the file into memory. */
  fread(machine->memory, sizeof(uint32_t), 16384, file);

  /* Close the file after reading */
  fclose(file); 
}
