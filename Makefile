debug ?= false
ifeq (${debug}, true)
	DEBUG_FLAGS := -g3 -ggdb -ftrapv -fsanitize=address -fsanitize=leak -fsanitize=undefined
endif

SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst %.c,%.o,${SOURCES})
DEPENDENCIES := $(patsubst %.c,%.d,${SOURCES})

INCLUDE_FLAGS := -I./include
WARNING_FLAGS := -Wextra -Wall -Wshadow -Wdouble-promotion -Wpadded \
	-Wformat=2 -Wformat-truncation -fno-common -Wconversion
CFLAGS += ${WARNING_FLAGS} ${INCLUDE_FLAGS} ${DEBUG_FLAGS}
export

.PHONY: clean tests coverage

${OBJECTS}: %.o: %.c
	${CC} -Werror ${CFLAGS} -MMD -c $< -o $@

tests: ${OBJECTS}
	${MAKE} -C tests tests
	./tests/tests

-include $(DEPENDENCIES)

clean:
	rm ${OBJECTS} ${DEPENDENCIES}
	-$(MAKE) -C tests clean
