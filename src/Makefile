CC = -Wall -Werror -Wextra -std=c++17 -pedantic
CPP = -lstdc++
OS := $(shell uname)

ifeq ($(OS),Linux)
FLAGS = -lgtest -lm -lpthread -lrt -fprofile-arcs -ftest-coverage
else
FLAGS = -lgtest -lm -lpthread -fprofile-arcs -ftest-coverage
endif

all: clean s21_matrix_oop.a test

clean:
	rm -rf *.o *.a unit_test
	rm -rf *.gcno *gcda *.gcov gcov
	rm -rf report report.info

test: clean s21_matrix_oop.a
	gcc $(CC) test.cc s21_matrix_oop.a $(FLAGS) $(CPP) -o unit_test
	./unit_test

s21_matrix_oop.a:
	gcc $(CC) $(CPP) -c s21_matrix_oop.cc
	ar -crs s21_matrix_oop.a s21_matrix_oop.o

style:
	@ cp ../materials/linters/.clang-format ./
	clang-format -n *.cc *.h
	@ rm -rf .clang-format

leak:
	leaks -atExit -- ./unit_test

gcov_report: clean
	gcc ./*.cc $(FLAGS) $(CPP) -o unit_test
	./unit_test
	lcov -t "./unit_test"  -o report.info --no-external -c -d .
	genhtml -o report report.info
	open ./report/index.html
	rm -rf *.gcno *gcda *.gco

valgrind: test
	valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./unit_test
