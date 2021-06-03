CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

ifeq ($(shell uname), Darwin)
  CC = gcc-9
endif

SRC_DIRS ?= .

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I, $(INC_DIRS:./%=%))
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

# SRCS := $(shell find $(SRC_DIRS) -name "*.c")
GET_OBJS = $(addsuffix .o, $(basename $(shell find $(1) -name "*.c")))
DEPS := $($(call GET_OBJS,$(SRC_DIRS)):.o=.d)

EMULATE_SRC_DIRS := emulate_lib emulate.c
EMULATE_OBJS := $(call GET_OBJS, $(EMULATE_SRC_DIRS))

ASSEMBLE_SRC_DIRS := assemble_lib assemble.c
ASSEMBLE_OBJS := $(call GET_OBJS, $(ASSEMBLE_SRC_DIRS))

TEST_SRC_DIRS := emulate_lib assemble_lib testing testrunner.c
TEST_OBJS := $(call GET_OBJS, $(TEST_SRC_DIRS))

TO_CLEAN := $(shell find $(SRC_DIRS) -name "*.o" -or -name "*.d")

.SUFFIXES: .c .o
.PHONY: all clean

all: assemble emulate testrunner
	  rm -f $(TO_CLEAN)

tests: testrunner

emulate: $(EMULATE_OBJS)

assemble: $(ASSEMBLE_OBJS)

testrunner: $(TEST_OBJS)

clean:
	rm -f $(TO_CLEAN)
	rm -f assemble
	rm -f emulate
	rm -f testrunner

-include $(DEPS)
