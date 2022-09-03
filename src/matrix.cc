#include "s21_matrix_oop.h"

S21Matrix::S21Matrix() {
  // Default constructor creates 3x3 zero-matrix
  rows_ = 3;
  cols_ = 3;
  p_ = new double*[rows_];
  for (int i = 0; i < rows_; ++i) p_[i] = new double[cols_]();
}

S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  // This constructor creates rowsxcols zero-matrix
  // Note: rows_(rows) is a shortcut instead of rows_ = rows; in a separate line
  if (rows > 0 && cols > 0) {
    p_ = new double*[rows_];
    for (int i = 0; i < rows_; ++i) p_[i] = new double[cols_]();
  } else {
    throw CustomException("Rows and cols must be not less that 1");
  }
}

S21Matrix::S21Matrix(const S21Matrix& other)
    : S21Matrix::S21Matrix(other.rows_, other.cols_) {
  // This constructor creates a copy of a given matrix
  // Note: ":" syntax in this case invokes other member-function
  for (int i = 0; i < rows_; ++i)
    for (int j = 0; j < cols_; ++j) p_[i][j] = other.p_[i][j];
}

S21Matrix::S21Matrix(S21Matrix&& other) {
  // This constructor moves given matrix to this one
  // Note: this constructor invokes by compilator's decision when an other
  // object is about to be destroyed (e.g. when returning an object out of
  // function)
  rows_ = other.rows_;
  cols_ = other.cols_;
  p_ = other.p_;
  other.rows_ = 0;
  other.cols_ = 0;
  other.p_ = nullptr;
}

S21Matrix::~S21Matrix() {
  // Destructor just deallocates memory of p_
  if (p_) {
    for (int i = 0; i < rows_; ++i) delete[] p_[i];
    delete[] p_;
  }
}

void S21Matrix::set_rows(int rows) {
  // This mutator changes the rows_ value and reallocate p_ (if rows > rows_,
  // new matrix values will be filled with zeroes)
  if (rows < 1) throw CustomException("Rows cant be less than 1");
  if (rows < rows_) {
    for (int i = rows; i < rows_; ++i) delete[] p_[i];
  } else if (rows > rows_) {
    double** p = new double*[rows];
    for (int i = 0; i < rows; ++i) {
      p[i] = new double[cols_]();
      if (i < rows_)
        for (int j = 0; j < cols_; ++j) p[i][j] = p_[i][j];
    }
    this->~S21Matrix();
    p_ = p;
  }
  rows_ = rows;
}

void S21Matrix::set_cols(int cols) {
  // Similar to set_rows()
  if (cols < 1) throw CustomException("Columns cant be less than 1");
  if (cols != cols_) {
    for (int i = 0; i < rows_; ++i) {
      double* pi = new double[cols]();
      for (int j = 0; j < cols; ++j)
        if (j < cols_) pi[j] = p_[i][j];
      delete[] p_[i];
      p_[i] = pi;
    }
    cols_ = cols;
  }
}

bool S21Matrix::EqMatrix(const S21Matrix& other) {
  // This function returns true if cols, rows and matrix values of both matrixes
  // are equal, false otherwise
  bool res = true;
  if (rows_ != other.rows_ || cols_ != other.cols_) res = false;
  for (int i = 0; i < rows_ && res; ++i)
    for (int j = 0; j < cols_ && res; ++j)
      if (fabs(p_[i][j] - other.p_[i][j]) > 1e-7) res = false;
  return res;
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  // This function simply adds other matrix to this
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw CustomException("Different matrix dimensions");
  for (int i = 0; i < rows_; ++i)
    for (int j = 0; j < cols_; ++j) p_[i][j] += other.p_[i][j];
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  // This function simply subs other matrix from this
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw CustomException("Different matrix dimensions");
  for (int i = 0; i < rows_; ++i)
    for (int j = 0; j < cols_; ++j) p_[i][j] -= other.p_[i][j];
}

void S21Matrix::MulNumber(const double num) {
  // This function simply multiplies matrix values by number
  for (int i = 0; i < rows_; ++i)
    for (int j = 0; j < cols_; ++j) p_[i][j] *= num;
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  // This function multiplies this matrix by other, and sets a proper size to
  // this
  if (cols_ != other.rows_)
    throw CustomException(
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
  // This function return trasposed version of this matrix
  S21Matrix res(cols_, rows_);
  for (int i = 0; i < rows_; ++i)
    for (int j = 0; j < cols_; ++j) res.p_[j][i] = p_[i][j];
  return res;
}

S21Matrix S21Matrix::HandleMatrix(int ex_i, int ex_j) {
  // Little function to create a minor matrix from a given one by excluding
  // certain row and col
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
  // This function returns a matrix of algebraic complements of this matrix
  if (rows_ != cols_) throw CustomException("The matrix is not square");
  if (rows_ == 1) return *this;
  S21Matrix res(rows_, cols_);
  for (int i = 0; i < rows_; ++i)
    for (int j = 0; j < cols_; ++j) {
      S21Matrix sub_matrix = this->HandleMatrix(i, j);  // sub_matrix is a minor
      if (sub_matrix.rows_ == 2) {
        res.p_[i][j] = ((i + j) % 2 ? -1 : 1) * sub_matrix.TwoDet();
      } else if (sub_matrix.rows_ == 1) {
        res.p_[i][j] = ((i + j) % 2 ? -1 : 1) * sub_matrix.OneDet();
      } else {
        S21Matrix recursive =
            sub_matrix.CalcComplements();  // this is a recursive algorithm
        for (int k = 0; k < sub_matrix.rows_; ++k)
          res.p_[i][j] += sub_matrix.p_[0][k] * recursive.p_[0][k];
        if ((i + j) % 2) res.p_[i][j] *= -1;
      }
    }
  return res;
}

double S21Matrix::Determinant() {
  // This function reterns a determinant of this matrix
  if (rows_ != cols_) throw CustomException("The matrix is not square");
  S21Matrix complement = this->CalcComplements();
  double res = 0;
  for (int i = 0; i < rows_; ++i) res += p_[0][i] * complement.p_[0][i];
  return res;
}

S21Matrix S21Matrix::InverseMatrix() {
  // This function reterns an inverse matrix of this matrix
  double det = this->Determinant();
  if (fabs(det) < 1e-7) throw CustomException("Matrix determinant is 0");
  S21Matrix complement = this->CalcComplements();
  S21Matrix trasposed = complement.Transpose();
  trasposed.MulNumber(1 / det);
  return trasposed;
}

double& S21Matrix::operator()(int row, int col) {
  // This operator is a mutator of matrix values
  if (row >= rows_ || col >= cols_ || col < 0 || row < 0)
    throw std::out_of_range("Incorrect input, index is out of range");
  return p_[row][col];
}

double& S21Matrix::operator()(int row, int col) const {
  // This operator is an accessor of matrix values
  if (row >= rows_ || col >= cols_ || col < 0 || row < 0)
    throw std::out_of_range("Incorrect input, index is out of range");
  return p_[row][col];
}

bool S21Matrix::operator==(const S21Matrix& other) {
  return this->EqMatrix(other);
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  // This operator assign other matrix to this
  if (this == &other) return *this;  // Protection against self assignment
  this->set_cols(other.cols_);
  this->set_rows(other.rows_);
  for (int i = 0; i < rows_; ++i)
    for (int j = 0; j < cols_; ++j) p_[i][j] = other.p_[i][j];
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
