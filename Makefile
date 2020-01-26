# Common to test and the actual app
CXX_FLAGS=-Werror -Wall -Wpedantic -Wextra
STD=--std=c++17

# Main app
BUILD_DIR=${APP_NAME}_build
SRC_DIR=src
APP_NAME=interpreter
SRC=$(wildcard ${SRC_DIR}/*.cpp)
OBJECTS=$(SRC:${SRC_DIR}/%.cpp=${BUILD_DIR}/%.o)

# Test app
TEST_DIR=test
TEST_SRC=$(wildcard ${TEST_DIR}/*.cpp)
TEST_OBJS=$(TEST_SRC:${TEST_DIR}/%.cpp=${TEST_BUILD_DIR}/%.o)
TEST_APP=test_app
TEST_BUILD_DIR=${TEST_APP}_build

# Conan stuff
-include ${BUILD_DIR}/conanbuildinfo.mak
INCLUDE_PATHS=$(addprefix -I, ${SRC_DIR} ${CONAN_INCLUDE_DIRS})
LIBS=$(addprefix -l, ${CONAN_LIBS})
LIB_PATH=$(addprefix -L, ${CONAN_LIB_DIRS})
DEFINES=$(addprefix -D, ${CONAN_DEFINES})

.PHONY: clean
.PHONY: setup
.PHONY: format
.PHONY: check
.PHONY: test

# Different optimization levels for testing, as I spent time debugging
${TEST_APP}: OPTIMIZE=-O0 -g
${APP_NAME}: OPTIMIZE=-O3

all: ${APP_NAME} ${TEST_APP}

setup:
	@mkdir -p ${BUILD_DIR} && cd ${BUILD_DIR} && conan install -g make ..
	@mkdir -p ${TEST_BUILD_DIR}
	@echo "Setup has been run, so you may have to run you command again if it fails"

format:
	clang-format -i ${SRC} ${TEST_SRC}

check:
	cppcheck --enable=all ${STD} ${INCLUDE_PATHS} ${SRC}

${TEST_APP}: ${TEST_OBJS}
	clang++ -o $@ $^ ${LIBS} ${LIB_PATH} 

test: ${TEST_APP}
	./$^

${APP_NAME}: ${OBJECTS}
	clang++ -o $@ $^ ${LIBS} ${LIB_PATH} 

${BUILD_DIR}/%.o: ${SRC_DIR}/%.cpp
	clang++ -c -o $@ $< ${CXX_FLAGS} ${INCLUDE_PATHS} ${CONAN_CXXFLAGS} ${DEFINES} ${STD} ${OPTIMIZE}

${TEST_BUILD_DIR}/%.o: ${TEST_DIR}/%.cpp
	clang++ -c -o $@ $< ${CXX_FLAGS} ${INCLUDE_PATHS} ${CONAN_CXXFLAGS} ${DEFINES} ${STD} ${OPTIMIZE}

clean:
	-rm ${OBJECTS}
	-rm ${TEST_OBJS}
	-rm ${APP_NAME}
	-rm ${TEST_APP}
