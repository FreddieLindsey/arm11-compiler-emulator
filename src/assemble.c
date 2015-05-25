#include <stdlib.h>
#include "assemble_utils/secondpass.h"

int main(int argc, char **argv) {

  if(argc != 2) {
    printf("Usage: assemble <file>");
    return EXIT_FAILURE;
  }

  FILE *file = fopen(argv[1], "r");
  //first pass symbol table
  //second pass convert to binary
  //write binary file
  return EXIT_SUCCESS;
}
