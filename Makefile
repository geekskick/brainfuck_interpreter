CXX_FLAGS=-g -O0 -Werror -Wall -Wpedantic -Wextra -std=c++17
BUILD_DIR=${APP_NAME}_build
SRC_DIR=src
APP_NAME=interpreter

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
.PHONY: ${APP_NAME}

all: ${APP_NAME} ${TEST_APP}

info: setup
	@echo "APP SRC = ${SRC}"
	@echo "TEST SRC= ${TEST_SRC}"
	@echo "OBJECTS = ${OBJECTS}"
	@echo "IPath   = ${INCLUDE_PATHS}"
	@echo "FLAGS   = ${CXX_FLAGS}"
	@echo "LIBS    = ${LIBS}"
	@echo "DEFINES = ${DEFINES}"
	@echo "LPath   = ${LIB_PATH}"
	@echo "APP NAME=${APP_NAME}"
	@echo "TST NAME=${TEST_APP}"
	@echo "TST OBJS=${TEST_OBJS}"

setup:
	@mkdir -p ${BUILD_DIR} && cd ${BUILD_DIR} && conan install -g make ..
	@mkdir -p ${TEST_BUILD_DIR}
	@echo "Setup has been run, so you may have to run you command again if it fails"

format:
	clang-format -i ${SRC}

check:
	cppcheck -enable=all ${SRC}

${TEST_APP}: ${TEST_OBJS}
	clang++ -o $@ $^ ${LIBS} ${LIB_PATH} 

test: ${TEST_APP}
	./$^

${APP_NAME}: ${OBJECTS}
	clang++ -o $@ $^ ${LIBS} ${LIB_PATH} 

${BUILD_DIR}/%.o: ${SRC_DIR}/%.cpp
	clang++ -c -o $@ $< ${CXX_FLAGS} ${INCLUDE_PATHS} ${CONAN_CXXFLAGS} ${DEFINES}

${TEST_BUILD_DIR}/%.o: ${TEST_DIR}/%.cpp
	clang++ -c -o $@ $< ${CXX_FLAGS} ${INCLUDE_PATHS} ${CONAN_CXXFLAGS} ${DEFINES}

clean:
	-rm -r ${BUILD_DIR}
	-rm -r ${TEST_BUILD_DIR}
	-rm ${APP_NAME}
	-rm ${TEST_APP}
