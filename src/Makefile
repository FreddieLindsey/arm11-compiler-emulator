CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o assemble_/secondpass.c assemble_/instructions.c
	$(CC) $(CFLAGS) assemble.c assemble_/secondpass.c assemble_/instructions.c -o assemble

emulate: emulate_/machine/machine.c emulate_/machine/fileio.c emulate_/pipeline.c instructions/_decoder.c instructions/_fetch.c
	$(CC) $(CFLAGS) emulate_/machine/machine.c emulate_/machine/fileio.c emulate_/pipeline.c instructions/_decoder.c instructions/_fetch.c instructions/_executor.c emulate.c -o emulate

clean:
	rm -f $(wildcard *.o)
	rm -rf $(wildcard *.dSYM)
	rm -f assemble
	rm -f emulate
