#silent mode

# ifndef VERBOSE
# .SILENT:
# endif

#Compile vars
CC = gcc
CC_FLAGS = -Wall -Wextra #-Werror
DEBUG_FLAGS = -std=c11 -g #TODO: clear debug
GCOV_FLAGS = --coverage
TEST_COMPILE = $(CC) $(CC_FLAGS) $(TEST_SOURCE) $(LIB_NAME).a $(CHECK_FLAGS) $(DEBUG_FLAGS) -I$(SRC_DIR) -I$(TEST_DIR)
GCOV_COMPILE = $(CC) $(CC_FLAGS) $(TEST_SOURCE) $(SOURCES) $(CHECK_FLAGS) $(GCOV_FLAGS) -I$(SRC_DIR) -I$(TEST_DIR)

#Files vars
LIB_NAME = s21_decimal
SRC_DIR = .
SOURCES = $(wildcard $(SRC_DIR)/*.c)
TEST_DIR = tests/
TEST_SOURCE = $(TEST_DIR)s21_test.c

#TODO: install requered soft

# REQUIRED_BINS := gcc
# $(foreach bin,$(REQUIRED_BINS),\
#     $(if $(shell command -v $(bin) 2> /dev/null),,$(error Please install `$(bin)`)))

#Common vars
OS = $(shell uname)
CODE_STYLE=-style=Google

#System vars
ifeq ($(OS), Linux)
	CHECK_FLAGS = -lcheck -pthread -lrt -lm -lsubunit
else
	CHECK_FLAGS = -lcheck `pkg-config --cflags --libs check`
endif

all: $(LIB_NAME).a

rebuild: clean all

# it's must not be named with 'lib' if it's only one static lib in the same dir
test: $(LIB_NAME).a _unittests-forking_disable
	$(TEST_COMPILE) -o test.out
	./test.out

gcov_report: $(LIB_NAME).a _unittests-forking_disable
	$(GCOV_COMPILE) -o report.out 
	./report.out
# gcov -f $(SOURCES) TODO: understand how it works
	lcov -q -t report.out -o report.info -c -d .
	genhtml -q -o report report.info
	rm -rf *.gcda *.gcno *.info *.out
	open ./report/index.html
	
check: $(LIB_NAME).a _unittests-forking_disable
	$(TEST_COMPILE) -o test.out
	clang-format -n *.h $(SOURCES)
ifeq ($(OS), Linux)
	touch valgrind_res.txt
	valgrind -s --leak-check=full --log-file="valgrind_res.txt" --track-origins=yes --show-reachable=yes ./test.out
	grep "ERROR" valgrind_res.txt
endif

$(LIB_NAME).a: $(LIB_NAME).o
	ar rcs $(LIB_NAME).a *.o
#	rm -rf *.o
	ranlib $(LIB_NAME).a

$(LIB_NAME).o:
	$(CC) $(CC_FLAGS) -c $(SOURCES) $(DEBUG_FLAGS)

clean: 
	rm -rf *.a *.o *.out report valgrind_res.txt tests/*.c *.dSYM

include check.mk

.PHONY: clean