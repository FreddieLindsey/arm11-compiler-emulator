#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <libgen.h>
#include <string.h>
#include "emulate_/funcprotos.h"

/*
 *  VARIABLES
 */



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
  } else if (argc > 2 && (strcmp(argv[2], "v") == 0 
                       || strcmp(argv[2],"V") == 0)) {
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
  init_machine(&machine);  
  if (verbose) printf("Machine initialised.\n");

  /*  Load a file into machine memory, then close it */
  if (verbose) printf("Loading file %s.\n", argv[1]);
  loadfile(file, &machine);
  if (verbose) printf("Loaded file %s.\n", argv[1]);

  /*  Print the state of the machine */
  if (verbose) print_machine(&machine);

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
