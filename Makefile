debug ?= false
ifeq (${debug}, true)
	DEBUG_FLAGS := -g3 -ggdb -ftrapv -fsanitize=address -fsanitize=leak -fsanitize=undefined
endif

profile ?= false
ifeq (${profile}, true)
	DEBUG_FLAGS += -pg
endif

SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst %.c,%.o,${SOURCES})
DEPENDENCIES := $(patsubst %.c,%.d,${SOURCES})

INCLUDE_FLAGS := -I./include
WARNING_FLAGS := -Wextra -Wall -Wshadow -Wdouble-promotion -Wpadded \
	-Wformat=2 -Wformat-truncation -fno-common -Wconversion
CFLAGS += ${WARNING_FLAGS} ${INCLUDE_FLAGS} ${DEBUG_FLAGS}
export

.PHONY: clean tests coverage library

${OBJECTS}: %.o: %.c
	${CC} -Werror ${CFLAGS} -MMD -c $< -o $@

tests: ${OBJECTS}
	${MAKE} -C tests tests
	./tests/tests

library: ${OBJECTS} | lib
	ar -rc lib/libdictionary.a $^

lib:
	mkdir lib

-include $(DEPENDENCIES)

clean:
	rm -rf ${OBJECTS} ${DEPENDENCIES} lib
	-$(MAKE) -C tests clean
