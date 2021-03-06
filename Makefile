CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -Werror -pedantic
LDFLAGS := $(LDFLAGS)-L/usr/local/lib/
LDLIBS  = -lnng -lbinn -lsodium

SRC_DIRS ?= .

INC_DIRS := $(shell find . ! -path . -type d)

ifeq ($(shell uname), Darwin)
  CC = gcc-9
  INC_DIRS := $(INC_DIRS) /usr/local/include/nng/ \
	/usr/local/include/ /usr/local/include/sodium/
endif

INC_FLAGS := $(addprefix -I, $(INC_DIRS:./%=%))
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP


# SRCS := $(shell find $(SRC_DIRS) -name "*.c")
GET_OBJS = $(addsuffix .o, $(basename $(shell find $(1) -name "*.c")))
DEPS := $($(call GET_OBJS,$(SRC_DIRS)):.o=.d)

NODE_SRC_DIRS := lib
NODE_OBJS := $(call GET_OBJS, $(NODE_SRC_DIRS))

TEST_SRC_DIRS := test
TEST_OBJS := $(call GET_OBJS, $(TEST_SRC_DIRS))

.SUFFIXES: .c .o
.PHONY: all clean

all: node address_node testrunner
	$(RM) $(shell find $(SRC_DIRS) -name "*.d")

test: testrunner
	$(RM) $(shell find $(SRC_DIRS) -name "*.d")
	./testrunner

node: $(NODE_OBJS)

address_node: $(NODE_OBJS)

testrunner: $(NODE_OBJS) $(TEST_OBJS)

clean:
	$(RM) $(shell find $(SRC_DIRS) -name "*.o")
	$(RM) $(shell find $(SRC_DIRS) -name "*.d")
	$(RM) $(shell find $(SRC_DIRS) -name "*.bin")
	rm -rf $(shell find $(SRC_DIRS) -name "*.dSYM")
	$(RM) node
	$(RM) testrunner
	$(RM) address_node


-include $(DEPS)
