CC = g++ -std=c++17 -Wall -Werror -Wextra -pedantic
SOURCE = s21*.cc
OBJECT = $(patsubst %s21*.cc, %*.o,  ${SOURCE})
TEST_FLAGS =-lgtest

ifeq ($(OS), Darwin)
 CC += -D OS_MAC
endif
ifeq ($(OS), Linux)
 CC += -D OS_LINUX
 TEST_FLAGS += -lrt -lm -lc++
endif

all: clean s21_matrix_oop.a

s21_matrix_oop.a:$(SOURCE)
	$(CC) -c $(SOURCE)
	@ar rcs s21_matrix_oop.a $(OBJECT)

test: s21_matrix_oop.a
	$(CC) test.cc s21_matrix_oop.a $(TEST_FLAGS) -o test
	./test

clang:
	clang-format -style=Google -n *.cc  *.h

clean:
	@rm -rf *.o *.a test