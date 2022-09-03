#ifndef SRC_S21_MATRIX_OOP_H_
#define SRC_S21_MATRIX_OOP_H_

#include <cmath>
#include <exception>
#include <iostream>

class CustomException : public std::exception {
  // Custom exception class
 private:
  char const* message_;

 public:
  // Constructor simply copies the message given to it to a fieild of a class
  explicit CustomException(char const* msg) { message_ = msg; }
  // What is default exception function that returns an explanatory string
  char const* what() { return message_; }
};

class S21Matrix {
 private:
  int rows_, cols_;
  double** p_;

  // Some hidden function, needed by CalcComplements()
  int TwoDet() { return p_[0][0] * p_[1][1] - p_[0][1] * p_[1][0]; }
  int OneDet() { return p_[0][0]; };
  S21Matrix HandleMatrix(int ex_i, int ex_j);

 public:
  // Constructors and destructor
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other);
  ~S21Matrix();

  // Accessors and mutators of rows_ and cols_ fields
  int get_rows() { return rows_; };
  int get_cols() { return cols_; };
  void set_rows(int rows);
  void set_cols(int cols);

  // Common matrix operations
  bool EqMatrix(const S21Matrix& other);
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose();
  S21Matrix CalcComplements();
  double Determinant();
  S21Matrix InverseMatrix();

  // Overloaded operators
  double& operator()(int row, int col);
  double& operator()(int row, int col) const;
  bool operator==(const S21Matrix& other);
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix operator+(const S21Matrix& other);
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix operator-(const S21Matrix& other);
  S21Matrix& operator*=(const S21Matrix& other);
  S21Matrix operator*(const S21Matrix& other);
  S21Matrix& operator*=(const double num);
  friend S21Matrix operator*(double num, const S21Matrix& this_m);
  friend S21Matrix operator*(const S21Matrix& this_m, double num);

  // Simple function for debug (never used in inmplementation)
  void PrintMatrix() {
    for (int i = 0; i < rows_; ++i)
      for (int j = 0; j < cols_; ++j) std::cout << p_[i][j] << std::endl;
  }
};

// This functions needed to implement MulNumber() in any order relatively to
// matrix (e.g. 2 * m and m * 2)
S21Matrix operator*(double num, const S21Matrix& this_m);
S21Matrix operator*(const S21Matrix& this_m, double num);

#endif  // SRC_S21_MATRIX_OOP_H_
