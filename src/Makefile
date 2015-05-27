CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean machine

all: assemble emulate

assemble: assemble.o assemble_/secondpass.c assemble_/instructions.c
	$(CC) $(CFLAGS) assemble.c assemble_/secondpass.c assemble_/instructions.c -o assemble

emulate: emulate.o emulate_/machine/machine.c emulate_/machine/fileio.c

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
