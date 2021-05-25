CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

ifeq ($(shell uname), Darwin)
  CC = gcc-9
endif

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate: emulate.o emulate_utils.o decoder.o

emulate_ioutils.o: emulate_utils.h

decoder: decoder.h

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
