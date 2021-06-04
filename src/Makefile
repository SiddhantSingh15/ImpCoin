CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

ifeq ($(shell uname), Darwin)
  CC = gcc-9
endif

SRC_DIRS ?= .

INC_DIRS := $(shell find . ! -path . -type d)
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

.SUFFIXES: .c .o
.PHONY: all clean

all: assemble emulate testrunner
	$(RM) $(shell find $(SRC_DIRS) -name "*.d")

tests: testrunner
	$(RM) $(shell find $(SRC_DIRS) -name "*.d")

emulate: $(EMULATE_OBJS)

assemble: $(ASSEMBLE_OBJS)

testrunner: $(TEST_OBJS)

clean:
	$(RM) $(shell find $(SRC_DIRS) -name "*.o")
	$(RM) $(shell find $(SRC_DIRS) -name "*.d")
	rm -rf $(shell find $(SRC_DIRS) -name "*.dSYM")
	$(RM) assemble
	$(RM) emulate
	$(RM) testrunner

-include $(DEPS)
