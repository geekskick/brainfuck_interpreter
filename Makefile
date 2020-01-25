CXX_FLAGS=-O0 -Werror -Wall -Wpedantic -Wextra -g
BUILD_DIR=${APP_NAME}_build
SRC_DIR=src
APP_NAME=interpreter
STD=--std=c++17
SRC=$(wildcard ${SRC_DIR}/*.cpp)
OBJECTS=$(SRC:${SRC_DIR}/%.cpp=${BUILD_DIR}/%.o)

-include ${BUILD_DIR}/conanbuildinfo.mak

INCLUDE_PATHS=$(addprefix -I, ${SRC_DIR} ${CONAN_INCLUDE_DIRS})
LIBS=$(addprefix -l, ${CONAN_LIBS})
LIB_PATH=$(addprefix -L, ${CONAN_LIB_DIRS})
DEFINES=$(addprefix -D, ${CONAN_DEFINES})
TEST_DIR=test
TEST_SRC=$(wildcard ${TEST_DIR}/*.cpp)
TEST_OBJS=$(TEST_SRC:${TEST_DIR}/%.cpp=${TEST_BUILD_DIR}/%.o)
TEST_APP=test_app
TEST_BUILD_DIR=${TEST_APP}_build

.PHONY: clean
.PHONY: setup
.PHONY: format
.PHONY: check
.PHONY: test

all: ${APP_NAME} ${TEST_APP}

info: setup
	@echo "APP SRC \t= ${SRC}"
	@echo "TEST SRC\t= ${TEST_SRC}"
	@echo "OBJECTS \t= ${OBJECTS}"
	@echo "IPath   \t= ${INCLUDE_PATHS}"
	@echo "FLAGS   \t= ${CXX_FLAGS}"
	@echo "LIBS    \t= ${LIBS}"
	@echo "DEFINES \t= ${DEFINES}"
	@echo "LPath   \t= ${LIB_PATH}"
	@echo "APP NAME\t= ${APP_NAME}"
	@echo "TST NAME\t= ${TEST_APP}"
	@echo "TST OBJS\t= ${TEST_OBJS}"

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
	clang++ -c -o $@ $< ${CXX_FLAGS} ${INCLUDE_PATHS} ${CONAN_CXXFLAGS} ${DEFINES} ${STD}

${TEST_BUILD_DIR}/%.o: ${TEST_DIR}/%.cpp
	clang++ -c -o $@ $< ${CXX_FLAGS} ${INCLUDE_PATHS} ${CONAN_CXXFLAGS} ${DEFINES} ${STD}

clean:
	-rm ${BUILD_DIR}/*.o
	-rm ${TEST_BUILD_DIR}/*.o
	-rm ${APP_NAME}
	-rm ${TEST_APP}
