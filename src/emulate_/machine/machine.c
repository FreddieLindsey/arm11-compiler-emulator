/*
 *  Prints current values of registers
 */
void printState(struct machine *machine) {
  // Registers
  printf("Registers:\n");
  for(int i = 0; i < 13; i++) {
    printf("$%d %s:\t\t%d (0x%08x)\n",
        i, (i < 10) ? " " : "", (*machine).r[i], (*machine).r[i]);
  }
  printf("PC  :\t\t%d (0x%08x)\n", (*machine).pc, (*machine).pc);
  printf("CPSR:\t\t%d (0x%08x)\n", (*machine).cpsr, (*machine).cpsr);

  // Memory
  printf("Non-zero memory:\n");
  for(long i = 0; i < 65536; i += 4) {
    char a = (*machine).memory[i];
    char b = (*machine).memory[i+1];
    char c = (*machine).memory[i+2];
    char d = (*machine).memory[i+3];
    if(a != 0 || b != 0 || c != 0 || d != 0) {
     printf("0x%08lx: 0x%02x%02x%02x%02x\n", i, a, b, c, d);
    }
  }
}