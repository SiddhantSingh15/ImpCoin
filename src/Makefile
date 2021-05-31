CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

ifeq ($(shell uname), Darwin)
  CC = gcc-9
endif

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate testrunner

tests: testrunner

assemble: assemble_lib/assemble_utils.o

assemble_lib/assemble_utils.o: assemble_lib/assemble_utils.h

emulate: emulate_lib/emulate_utils.o emulate_lib/decoder.o emulate_lib/exec_instructions.o emulate_lib/exec_utils.o emulate_lib/dataproc_operations.o

emulate_lib/emulate_utils.o: emulate_lib/emulate_utils.h

emulate_lib/decoder: emulate_lib/decoder.h

emulate_lib/exec_instructions.o: emulate_lib/exec_utils.o emulate_lib/exec_instructions.h emulate_lib/dataproc_operations.o

emulate_lib/dataproc_operations.o: emulate_lib/dataproc_operations.h

emulate_lib/exec_utils.o: emulate_lib/exec_utils.h

testrunner: testing/test_utils.o emulate_lib/decoder.o emulate_lib/emulate_utils.o emulate_lib/exec_utils.o

testing/test_utils: testing/test_utils.h

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
	rm -f emulate_lib/emulate_utils
	rm -f testrunner
