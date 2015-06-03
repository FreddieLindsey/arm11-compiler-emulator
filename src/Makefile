CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean assemble

all: assemble emulate

ASS_OBJS = assemble_/secondpass.c assemble_/build_instruction.c instructions/_encoder.c assemble.c
ASS_OUT = assemble
assemble:
	$(CC) $(CFLAGS) $(ASS_OBJS) -o $(ASS_OUT) 

EMU_OBJS = emulate_/machine/machine.c emulate_/machine/fileio.c emulate_/pipeline.c instructions/_decoder.c instructions/_fetch.c instructions/_executor.c emulate.c
EMU_OUT = emulate
emulate: 
	$(CC) $(CFLAGS) $(EMU_OBJS) -o $(EMU_OUT) 

clean:
	rm -f $(wildcard *.o)
	rm -rf $(wildcard *.dSYM)
	rm -f assemble
	rm -f emulate
