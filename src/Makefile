CC = g++

CFLAGS = -Wall -Wextra -Werror -g
GCOV_FLAGS := -fprofile-arcs -ftest-coverage
LDFLAGS := 

SOURCES:= matrix.cc

OBJ_DIR := ./obj
OBJECTS := $(addprefix obj/,$(SOURCES:.cc=.o))
HEADER = s21_matrix_oop.h

TARGET_EXEC := s21_matrix_oop.a

RM := rm -rf


all: $(TARGET_EXEC)

$(TARGET_EXEC): create_dir $(OBJECTS)
	ar rcs $@ $(OBJECTS)

create_dir:
	mkdir -p $(OBJ_DIR)

test: main_tests.c $(HEADER) $(TARGET_EXEC)
	$(CC) $(CFLAGS) $^ -o test_test $(LDFLAGS)
	./test_test

$(OBJ_DIR)/%.o: %.cc $(HEADER)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)
	$(RM) $(TARGET_EXEC)

lint:
	cp ../materials/linters/.clang-format ./
	clang-format -style=Google -n matrix.cc s21_matrix_oop.h
	$(RM) .clang-format

rebuild: clean all

.PHONY: all clean rebuild test lint create_dir