#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <libgen.h>
#include <string.h>
#include "emulate.h"

/*
 *  MAIN
 */

int main(int argc, char** argv) {

  /*  Constant that can be enabled for verbose mode */
  static int verbose = 0;

  /*  Check if a file has been given as an argument
      return if not */
  if (argc != 2 && !(argc == 3 && (strcmp(argv[2], "v") == 0))) {
    printf("You have supplied %s arguments.\nEXITING\n",
          (argc < 2) ? "too few" : "too many" );
    usage();
    exit(EXIT_FAILURE);
  } else if (argc == 3 && (strcmp(argv[2], "v") == 0
                        || strcmp(argv[2], "V") == 0)) {
    verbose = 1;
  }

  /*  Check a given filename exists, and return if not */
  FILE *file;
  if ((file = fopen(argv[1], "r")) == NULL) {
    perror("Error opening file!");
    printf("Does the file %s exist?\nEXITING\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  if (verbose) printf("File opened successfully:\t%s\n", argv[1]);

  /*  Create and initialise a machine and a pipeline */
  machine_t machine;
  init_machine(&machine);
  if (verbose) printf("Machine initialised.\n");
  init_pipeline(machine.pipeline);
  if (verbose) printf("Pipeline initialised.\n");

  /*  Load a file into machine memory, then close it */
  if (verbose) printf("Loading file\t\t\t%s\n", argv[1]);
  loadfile(file, &machine);
  if (verbose) printf("Loaded file\t\t\t%s\n", argv[1]);

  /*  Print the state of the machine prior to any execution */
  if (verbose) print_machine(&machine);

  /*  Execute the pipeline model of the ARM architecture */
  run_pipeline(&machine);
  close_pipeline(machine.pipeline);

  /*  Print the final state of the machine */
  print_machine(&machine);

  /*  Frees the machine by closing it */
  close_machine(&machine);
  if (verbose) printf("Machine closed, memory freed\n");

  return EXIT_SUCCESS;
}

/*
 *  FUNCTIONS
 */

void usage(void) {
  printf( "Program takes 1 compulsory argument and 1 optional one:"
          "\n\nExample:\n"
          "./emulate binary_arm_file\n"
          "OR\n"
          "./emulate binary_arm_file v\t\t[For verbose output]\n");
}
