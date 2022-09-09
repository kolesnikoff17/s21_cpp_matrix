## Implementation of the s21_matrix_oop.h library functions

- The program must be developed in C++ language of C++17 standard using gcc compiler
- The program code must be located in the src folder
- When writing code it is necessary to follow the Google style
- Implement the matrix as an `S21Matrix` class
- Use only the `matrix_`, `rows_` and `cols_` fields as private.
- Implement the access to private fields `rows_` and `cols_` via accessor and mutator. If the matrix increases in size, it is filled with zeros. If it decreases in size, the excess is simply discarded.
- Make it as a static library (with s21_matrix_oop.h header file)
- Overload the operators
- Prepare full coverage of library functions code with unit-tests using the GTest library
- Provide a Makefile for building the library and tests (with targets all, clean, test, s21_matrix_oop.a)

