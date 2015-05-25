typedef struct machine {
  unsigned char *memory;
  int *r; // General purpose registers
  int pc; // Other registers
  int cpsr;
} machine_t;
