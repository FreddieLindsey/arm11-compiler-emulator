/*
 *  Converts a byte to a binary string
 *  Helper function to print out a byte for debugging
 */
char* byteToBinary(int x) {
  static char buffer[9];
  buffer[0] = '\0';

  for(int i = 128; i > 0; i >>= 1) {
    strcat(buffer, ((x & i) == i) ? "1" : "0");
  }

  return buffer;
}
