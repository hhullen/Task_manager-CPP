MAIN_PROJ_NAME=TaskManager
TEST_DIR=tests
LIB_NAME=task_manager
BUILD_FILES=$(LIB_NAME).h
TESTBUILD_FILES=$(TEST_DIR)$(SEP)$(MAIN_PROJ_NAME)_tests.cc
TEST_EXECUTABLE=$(MAIN_PROJ_NAME)_test.out
COMPILER=g++
STD=--std=c++20
CODE_DIAG_FLAGS=-Wall -Wextra -Werror -Wpedantic -Wshadow -Wconversion -Wnull-dereference -Wsign-conversion
TEST_FLAGS=-lgtest -pthread -lgtest_main
CLANG_FILE=.clang-format
CLANG_FILE_WAY=..$(SEP)materials$(SEP)linters$(SEP)$(CLANG_FILE)
CHECK_FILES=$(BUILD_FILES) taks_queue.h
CPPCH_SETUP=--enable=all --suppress=missingInclude \
			--suppress=unusedFunction -v --language=c++ $(STD)
TO_DELETE_FILES=*.o *.out *.dSYM *.gch *.gcda *.gcno .DS_Store $(TEST_EXECUTABLE) \
				$(CLANG_FILE) *.info
TO_DELETE_FOLDERS=report a.out.dSYM $(LIB_NAME) *.dSYM

#Crossplatform specs
SEP='\'
MAKEDIR=md
DELETE_FOLDER=RD /S/Q
DELETE_FILE=DEL /F
COPY=copy -Recurse
OPEN=start
OS=$(shell uname)

ifeq ($(OS), Linux)
	OPEN=xdg-open
	MAKEDIR=mkdir -p
	SEP=/
	DELETE_FOLDER=rm -rf
	DELETE_FILE=rm -f
	COPY=cp -r
endif

ifeq ($(OS), Darwin)
	OPEN=open
	MAKEDIR=mkdir -p
	SEP=/
	DELETE_FOLDER=rm -rf
	DELETE_FILE=rm -f
	COPY=cp -r
endif

.PHONY: all check tests clean lib

all: clean gcov_report tests check

check:
	cppcheck $(CPPCH_SETUP) $(CHECK_FILES)
	$(COPY) $(CLANG_FILE_WAY) $(CLANG_FILE)
	clang-format -i --style=Google $(CHECK_FILES)
	clang-format -n --style=Google $(CHECK_FILES)

tests:
	$(COMPILER) -g $(STD) $() $(TESTBUILD_FILES) $(TEST_FLAGS) \
	-o $(TEST_EXECUTABLE)
	.$(SEP)$(TEST_EXECUTABLE)

gcov_report: clean gcov_build
	lcov --ignore-errors mismatch -t "test" -o test.info -c -d .
	genhtml --ignore-errors mismatch -o report test.info

gcov_build:
	$(COMPILER) $(STD) $(CODE_DIAG_FLAGS) \
	$(TESTBUILD_FILES) $(TEST_FLAGS) \
	-o $(TEST_EXECUTABLE) --coverage
	.$(SEP)$(TEST_EXECUTABLE)

lib:
	$(MAKEDIR) $(LIB_NAME)
	$(COPY) $(BUILD_FILES) $(LIB_NAME)$(SEP)
	$(COPY) taks_queue.h $(LIB_NAME)$(SEP)

clean:
	$(DELETE_FOLDER) $(TO_DELETE_FOLDERS)
	$(DELETE_FILE) $(TO_DELETE_FILES)
