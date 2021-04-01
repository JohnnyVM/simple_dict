TEST_SOURCES := $(wildcard src/*.cpp)
TEST_OBJECTS := $(patsubst %.cpp,%.o,${TEST_SOURCES})
TEST_DEPENDENCIES := $(patsubst %.cpp,%.d,${TEST_SOURCES})
OBJECTS := $(patsubst %,../%,${OBJECTS})

INCLUDE_FLAGS := -I../include -I$(CPPUTEST_HOME)/include
LD_FLAGS += -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt
CPPFLAGS += ${INCLUDE_FLAGS} ${DEBUG_FLAGS}

tests: ${TEST_OBJECTS}
	${CXX} ${CPPFLAGS} -MMD -o $@ ${OBJECTS} ${TEST_OBJECTS} ${LD_FLAGS}

${TEST_OBJECTS}: %.o: %.cpp
	${CXX} ${CPPFLAGS} -MMD -c $< -o $@

-include $(TEST_DEPENDENCIES)

clean:
	rm ${TEST_OBJECTS} ${TEST_DEPENDENCIES}
