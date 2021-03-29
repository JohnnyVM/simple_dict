SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst %.c,%.o,${SOURCES})
DEPENDENCIES := $(patsubst %.c,%.d,${SOURCES})

COMMON_FLAGS := -Werror -Wextra -Wall -I./include
CFLAGS += ${COMMON_FLAGS}

CC ?= gcc

.PHONY: clean tests coverage

${OBJECTS}: %.o: %.c
	${CC} ${CFLAGS} -MMD -c $< -o $@

-include $(DEPENDENCIES)

clean:
	rm ${OBJECTS} ${DEPENDENCIES}
