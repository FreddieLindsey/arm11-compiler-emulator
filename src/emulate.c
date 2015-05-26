#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 *  STRUCTS / UNIONS
 */ 

typedef struct machine {
  char *memory;
  int *registers; 
} machine_t;

/*
 *  FUNCTION PROTOTYPES
 */

void usage(void);
void init_machine(machine_t *machine);
void printmachine(machine_t *machine);
void close_machine(machine_t *machine);
void loadfile(char *filename, machine_t *machine);

/*
 *  MAIN
 */

int main(int argc, char** argv) {

  /*  Check if a file has been given as an argument
      return if not */
  if (argc != 2) {
    usage();
    printf("You have supplied %s arguments.\nEXITING\n", 
          (argc < 2) ? "too few" : "too many" );
    return EXIT_FAILURE;
  }

  /*  Create and initialise a machine */
  machine_t machine;
  init_machine(&machine);  

  /*  Frees the machine by closing it */
  close_machine(&machine);
  
  return EXIT_SUCCESS;

}

/*
 *  FUNCTIONS
 */

void usage(void) {
  printf( "Program takes 1 argument:\n\nExample:\t"
          "./emulate binary_arm_file\n\n");
}

void init_machine(machine_t *machine) {
  machine->memory = malloc(65536);
  machine->registers = malloc(16 * sizeof(int));
}

void close_machine(machine_t *machine) {
  free(machine->memory);
  free(machine->registers);
}
