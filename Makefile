CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
NCURSES_LIB = -lncurses
GCOV_FLAGS = -ftest-coverage -fprofile-arcs -fPIC

BACK_SRC = brick_game/tetris/exe/backend.c brick_game/tetris/exe/fsm.c
FRONT_SRC = gui/cli/frontend.c
SRC_FILES := $(filter-out $(wildcard tests/*.c), $(wildcard *.c))

BACK = $(wildcard brick_game/tetris/exe/*.c)
FRONT = $(wildcard gui/cli/*.c)
OBJ_BACK = $(BACK:.c=.o)
OBJ_FRONT = $(FRONT:.c=.o)
TEST_FILES = tests/*.c

UNAME := $(shell uname -s)
ifeq ($(UNAME), Linux)
	CFLAGS += -lsubunit
	TEST_FLAGS = -pthread -lcheck_pic -pthread -lrt -lm -lsubunit
	ifneq ("$(wildcard /proc/version)", "")
		OPENER = wslview
	endif
else
	OPENER = xdg_open
endif
ifeq ($(UNAME), Darwin)
	TEST_FLAGS = $(shell pkg-config --cflags --libs check)
	OPENER = open
endif

all: install

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: back_lib front_lib
	$(CC) $(CFLAGS) -o TETRIS $(OBJ_BACK) $(OBJ_FRONT) $(NCURSES_LIB) $(GCOV_FLAGS)

back_lib: $(OBJ_BACK)
	ar rcs brick_game/tetris/libback.a $(OBJ_BACK)
	ranlib brick_game/tetris/libback.a

front_lib: $(OBJ_FRONT)
	ar rcs gui/cli/libfront.a $(OBJ_FRONT)
	ranlib gui/cli/libfront.a

uninstall:
	rm -f $(OBJ) TETRIS

dvi:
	pdflatex docs/doc.tex

dist: install
	rm -rf tetris.tar.gz
	tar -cvzf tetris.tar.gz TETRIS

test: clean back_lib front_lib $(TEST_FILES)
	$(CC) $(CFLAGS) $(TEST_FILES) brick_game/tetris/libback.a gui/cli/libfront.a -o test.out $(TEST_FLAGS) $(NCURSES_LIB)
	./test.out

gcov_report: test
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRC_FILES) $(BACK_SRC) $(FRONT_SRC) tests/check.c -o report.out $(NCURSES_LIB) $(TEST_FLAGS)
	./report.out
	lcov -t "check" -o check.info -c -d . --include '*/brick_game/tetris/exe/backend.c'
	genhtml -o ./report check.info
	rm -f *.gcno *.gcda *.info report.out
	$(OPENER) ./report/index-sort-f.html

clang-format:
	clang-format-12 -i -style=Google $(shell find . \( -name "*.c" -o -name "*.h" \))

clean:
	find . \( -name "*.o" -o -name "*.a" \) -type f -delete
	rm -f *.o *.a *.gcno *.gcda *.info *.out *.gcov tests/*.log tests/*.gcno tests/*.gcda *.txt
	rm -f TETRIS
	rm -rf ./report
	rm -rf tetris.tar.gz
	rm -f doc.*