#include <stdlib.h>
#include "loadfile.c"

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("Usage: emulate <file>");
    return EXIT_FAILURE;
  }
  loadfile(argv[1]);
  return EXIT_SUCCESS;
}
