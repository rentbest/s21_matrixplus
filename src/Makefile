CC = gcc -lstdc++ -g -Wall -Werror -Wextra
GCOVFLAGS = -fprofile-arcs -ftest-coverage
SOURSES = *.cc
OBJECT = $(patsubst %*.cc, %*.o,  ${SOURSES})
TEST_FLAGS =-lgtest -lgmock -pthread

ifeq ($(OS), Darwin)
 CC += -D OS_MAC
 ADD_LIB =
 TEST_FLAGS =-lgtest
endif
ifeq ($(OS), Linux)
 CC += -D OS_LINUX
 ADD_LIB = -lgtest -lm -lrt -lpthread
 TEST_FLAGS =-lpthread -lgtest -pthread -lrt -lm
endif

all: clean s21_matrix_oop.a

s21_matrix_oop.a:$(SOURSES)
	$(CC) -c $(SOURSES)
	@ar rcs s21_matrix_oop.a $(OBJECT)

test: s21_matrix_oop.a
	$(CC) tests.cc s21_matrix_oop.a $(TEST_FLAGS) -o unit-test
	./unit-test

gcov_report: clean s21_matrix_oop.a
	@gcc -lstdc++ --coverage -Wall -Werror -Wextra tests.cc s21_matrix_oop.cc -o GcovReport -lm -lgtest
	@./GcovReport
	@lcov -t "GcovReport" -o GcovReport.info -c -d ./ --no-external
	@genhtml -o report GcovReport.info
	@open ./report/index-sort-f.html
	@rm -rf ./*.gcno ./*.gcda ./GcovReport.*

clean:
	@rm -rf ./*.o ./*.a ./a.out ./GcovReport ./*.gcno ./*.gcda ./report ./*.info ./decimal_o ./*.dSYM ./unit-test ./CPPLINT*