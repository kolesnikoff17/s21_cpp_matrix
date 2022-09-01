#ifndef __S21MATRIX_H__
#define __S21MATRIX_H__

#include <cmath>
#include <exception>
#include <iostream>

// struct matrix_dimensions : public std::exception {
//   const char* what() const throw() { return "Different matrix dimensions"; }
// };

class S21Matrix {
 private:
  int rows_, cols_;
  double** p_;

  int TwoDet() { return p_[0][0] * p_[1][1] - p_[0][1] * p_[1][0]; }
  int OneDet() { return p_[0][0]; };
  S21Matrix HandleMatrix(int ex_i, int ex_j);

 public:
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other);
  ~S21Matrix();

  int get_rows() { return rows_; };
  int get_cols() { return cols_; };
  void set_rows(int rows);
  void set_cols(int cols);

  bool EqMatrix(const S21Matrix& other);
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose();
  S21Matrix CalcComplements();
  double Determinant();
  S21Matrix InverseMatrix();

  double operator()(int row, int col);
  double operator()(int row, int col) const;
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
};

S21Matrix operator*(double num, const S21Matrix& this_m);
S21Matrix operator*(const S21Matrix& this_m, double num);

#endif
