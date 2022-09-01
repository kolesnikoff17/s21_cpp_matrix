#include "s21_matrix_oop.h"

S21Matrix::S21Matrix() {
  rows_ = 3;
  cols_ = 3;
  p_ = new double*[rows_];
  for (auto i : {0, 1, 2}) p_[i] = new double[cols_]();
}

S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  p_ = new double*[rows_];
  for (int i = 0; i < rows_; ++i) p_[i] = new double[cols_]();
}

S21Matrix::S21Matrix(const S21Matrix& other)
    : S21Matrix::S21Matrix(other.rows_, other.cols_) {
  for (int i = 0; i < rows_; ++i)
    for (int j = 0; j < cols_; ++j) p_[i][j] = other.p_[i][j];
}

S21Matrix::S21Matrix(S21Matrix&& other) {
  rows_ = other.rows_;
  cols_ = other.cols_;
  p_ = other.p_;
  other.rows_ = 0;
  other.cols_ = 0;
  other.~S21Matrix();
}

S21Matrix::~S21Matrix() {
  if (p_) {
    for (int i = 0; i < rows_; ++i) delete[] p_[i];
    delete[] p_;
  }
}

void S21Matrix::set_rows(int rows) {
  if (rows < 1) throw std::invalid_argument("Rows cant be less than 1");
  if (rows < rows_) {
    for (int i = rows - 1; i < rows_; ++i) delete[] p_[i];
  } else if (rows > rows_) {
    double** p = new double*[rows];
    for (int i = 0; i < rows; ++i)
      if (i < rows_)
        for (int j = 0; j < cols_; ++j) p[i][j] = p_[i][j];
      else
        p[i] = new double[cols_]();
    delete[] p_;
    p_ = p;
  }
  rows_ = rows;
}

void S21Matrix::set_cols(int cols) {
  if (cols < 1) throw std::invalid_argument("Columns cant be less than 1");
  for (int i = 0; i < rows_; ++i) {
    double* pi = new double[cols]();
    for (int j = 0; j < cols; ++j)
      if (j < cols_) pi[j] = p_[i][j];
    delete[] p_[i];
    p_[i] = pi;
  }
  cols_ = cols;
}

bool S21Matrix::EqMatrix(const S21Matrix& other) {
  bool res = true;
  for (int i = 0; i < rows_ && res; ++i)
    for (int j = 0; j < cols_ && res; ++j)
      if (fabs(p_[i][j] - other.p_[i][j]) > 1e-7) res = false;
  return res;
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::invalid_argument("Different matrix dimensions");
  for (int i = 0; i < rows_; ++i)
    for (int j = 0; j < cols_; ++j) p_[i][j] += other.p_[i][j];
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::invalid_argument("Different matrix dimensions");
  for (int i = 0; i < rows_; ++i)
    for (int j = 0; j < cols_; ++j) p_[i][j] -= other.p_[i][j];
}

void S21Matrix::MulNumber(const double num) {
  for (int i = 0; i < rows_; ++i)
    for (int j = 0; j < cols_; ++j) p_[i][j] *= num;
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows_)
    throw std::invalid_argument(
        "The number of columns of the first matrix is not equal to the "
        "number of rows of the second matrix");
  S21Matrix this_copy(*this);
  this->set_cols(other.cols_);
  for (int i = 0; i < rows_; ++i)
    for (int j = 0; j < cols_; ++j) {
      double sum = 0;
      for (int k = 0; k < this_copy.cols_; ++k)
        sum += this_copy.p_[i][k] * other.p_[k][j];
      p_[i][j] = sum;
    }
}

S21Matrix S21Matrix::Transpose() {
  S21Matrix res(cols_, rows_);
  for (int i = 0; i < rows_; ++i)
    for (int j = 0; j < cols_; ++j) res.p_[j][i] = p_[i][j];
  return res;
}

S21Matrix S21Matrix::HandleMatrix(int ex_i, int ex_j) {
  S21Matrix res(rows_ - 1, cols_ - 1);
  for (int i = 0, i_b = 0; i < rows_; ++i, ++i_b) {
    if (i == ex_i) {
      --i_b;
      continue;
    }
    for (int j = 0, j_b = 0; j < cols_; ++j, ++j_b) {
      if (j == ex_j) {
        --j_b;
        continue;
      }
      res.p_[i_b][j_b] = p_[i][j];
    }
  }
  return res;
}

S21Matrix S21Matrix::CalcComplements() {
  if (rows_ != cols_) throw std::invalid_argument("The matrix is not square");
  if (rows_ == 1) return *this;
  S21Matrix res(rows_, cols_);
  S21Matrix sub_matrix;
  for (int i = 0; i < rows_; ++i)
    for (int j = 0; j < cols_; ++j) {
      sub_matrix = this->HandleMatrix(i, j);
      if (sub_matrix.rows_ == 2) {
        res.p_[i][j] = ((i + j) % 2 ? -1 : 1) * sub_matrix.TwoDet();
      } else {
        S21Matrix recursive = sub_matrix.CalcComplements();
        for (int k = 0; k < sub_matrix.rows_; ++k)
          res.p_[i][j] += sub_matrix.p_[0][k] * recursive.p_[0][k];
        if ((i + j) % 2) res.p_[i][j] *= -1;
      }
    }
  return res;
}

double S21Matrix::Determinant() {
  if (rows_ != cols_) throw std::invalid_argument("The matrix is not square");
  S21Matrix complement = this->CalcComplements();
  double res = 0;
  for (int i = 0; i < rows_; ++i) res += p_[0][i] * complement.p_[0][i];
  return res;
}

S21Matrix S21Matrix::InverseMatrix() {
  double det = this->Determinant();
  if (fabs(det) < 1e7) throw std::invalid_argument("Matrix determinant is 0");
  S21Matrix complement = this->CalcComplements();
  S21Matrix trasposed = complement.Transpose();
  trasposed.MulNumber(1 / det);
  return trasposed;
}

double S21Matrix::operator()(int row, int col) {
  if (row >= rows_ || col >= cols_)
    throw std::out_of_range("Incorrect input, index is out of range");
  return p_[row][col];
}

double S21Matrix::operator()(int row, int col) const {
  if (row >= rows_ || col >= cols_)
    throw std::out_of_range("Incorrect input, index is out of range");
  return p_[row][col];
}

bool S21Matrix::operator==(const S21Matrix& other) {
  return this->EqMatrix(other);
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  if (this == &other) return *this;
  cols_ = other.cols_;
  rows_ = other.rows_;
  this->~S21Matrix();
  p_ = other.p_;
  return *this;
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  this->SumMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) {
  S21Matrix res(*this);
  res.SumMatrix(other);
  return res;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  this->SubMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) {
  S21Matrix res(*this);
  res.SubMatrix(other);
  return res;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  this->MulMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) {
  S21Matrix res(*this);
  res.MulMatrix(other);
  return res;
}

S21Matrix& S21Matrix::operator*=(const double num) {
  this->MulNumber(num);
  return *this;
}

S21Matrix operator*(double num, const S21Matrix& this_m) {
  S21Matrix res(this_m);
  res.MulNumber(num);
  return res;
}

S21Matrix operator*(const S21Matrix& this_m, double num) {
  S21Matrix res(this_m);
  res.MulNumber(num);
  return res;
}
