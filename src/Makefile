CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

ifeq ($(shell uname), Darwin)
  CC = gcc-9
endif

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate testrunner

tests: testrunner

assemble: assemble.o

emulate: emulate.o emulate_utils.o decoder.o exec_instructions.o

emulate_utils.o: emulate_utils.h

decoder: decoder.h

exec_instructions.o: exec_instructions.h exec_utils.o emulate_utils.o

exec_utils.o: exec_utils.h

testrunner: testrunner.o test_utils.o decoder.o emulate_utils.o

test_utils: test_utils.h

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
	rm -f testrunner
