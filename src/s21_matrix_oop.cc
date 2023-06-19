#include "s21_matrix_oop.h"

// Default constructor
S21Matrix::S21Matrix() noexcept : rows_{}, cols_{}, matrix_{} {}

// Parameterized constructor
S21Matrix::S21Matrix(int rows, int cols) {
  if (rows < 1 || cols < 1) {
    throw std::invalid_argument("Rows or columns can't be less than 1");
  }
  rows_ = rows;
  cols_ = cols;
  InitMatrix_();
}

// Allocates memory for the matrix and initializes each cell with zero
void S21Matrix::InitMatrix_() {
  matrix_ = new double *[rows_];
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = new double[cols_];
  }
}

// Destructor
S21Matrix::~S21Matrix() { ClearMatrix_(); }

// Copy constructor
S21Matrix::S21Matrix(const S21Matrix &other)
    : rows_(other.rows_), cols_(other.cols_) {
  if (&other == this) {
    throw std::logic_error("Self-copying is not allowed");
  }
  CopyMatrix_(other);
}

// Copies the given matrix into the current matrix
void S21Matrix::CopyMatrix_(const S21Matrix &other) {
  InitMatrix_();
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

// Move constructor
S21Matrix::S21Matrix(S21Matrix &&other) noexcept {
  if (this != &other) {
    rows_ = std::exchange(other.rows_, 0);
    cols_ = std::exchange(other.cols_, 0);
    matrix_ = std::exchange(other.matrix_, nullptr);
  }
}

// Clears the memory and sets the number of rows and columns to zero
void S21Matrix::ClearMatrix_() noexcept {
  for (int i = 0; i < rows_; i++) {
    delete[] matrix_[i];
  }
  delete[] matrix_;
  matrix_ = {};
  rows_ = {};
  cols_ = {};
}

// Returns the number of rows in the matrix
int S21Matrix::GetRows() const noexcept { return rows_; }

// Returns the number of columns in the matrix
int S21Matrix::GetCols() const noexcept { return cols_; }

// Help function to copy matrix values
void S21Matrix::FillMatrix_(S21Matrix &newMatrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      newMatrix.matrix_[i][j] = matrix_[i][j];
    }
  }
}

// Sets the number of rows in the matrix (if greater than the current number of
// rows, initializes additional rows with zeros)
void S21Matrix::SetRows(int rows) {
  S21Matrix newMatrix(rows, cols_);
  double edge = rows_;
  if (rows < rows_) edge = rows;
  FillMatrix_(newMatrix, edge, cols_);
  *this = newMatrix;
}

// Sets the number of columns in the matrix (if greater than the current number
// of columns, initializes additional columns with zeros)
void S21Matrix::SetCols(int cols) {
  S21Matrix newMatrix(rows_, cols);
  double edge = cols_;
  if (cols < cols_) edge = cols;
  FillMatrix_(newMatrix, rows_, edge);
  *this = newMatrix;
}

// Checks if the matrix is equal to the given matrix
bool S21Matrix::EqMatrix(const S21Matrix &other) const noexcept {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    return false;
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        // 1.0e-07 is 10 * 10 ^ (-7)
        if (fabs(matrix_[i][j] - other.matrix_[i][j]) >= 1.0e-07) {
          return false;
        }
      }
    }
  }
  return true;
}

// Adds the given matrix to the current matrix
void S21Matrix::SumMatrix(const S21Matrix &other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::invalid_argument("Rows or columns are not equal");
  }
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

// Subtracts the given matrix from the current matrix
void S21Matrix::SubMatrix(const S21Matrix &other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::invalid_argument("Rows or columns are not equal");
  }
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

// Multiplies the matrix by a number
void S21Matrix::MulNumber(const double num) noexcept {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] *= num;
    }
  }
}

// Multiplies two matrices
void S21Matrix::MulMatrix(const S21Matrix &other) {
  if (cols_ != other.rows_) {
    throw std::invalid_argument("Invalid sizes of matrices for multiplying");
  }
  S21Matrix res(rows_, other.cols_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      res(i, j) = 0;
      for (int k = 0; k < cols_; k++) {
        res(i, j) += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  *this = res;
}

// Creates a transposed matrix from the current matrix and returns it
S21Matrix S21Matrix::Transpose() const {
  S21Matrix transposed(cols_, rows_);
  for (int i = 0; i < transposed.rows_; i++) {
    for (int j = 0; j < transposed.cols_; j++) {
      transposed.matrix_[i][j] = matrix_[j][i];
    }
  }
  return transposed;
}

// Calculates the matrix of cofactors of the current matrix and returns it
S21Matrix S21Matrix::CalcComplements() const {
  if (rows_ != cols_) {
    throw std::logic_error("The matrix is not square");
  }
  if (rows_ == 1) {
    throw std::logic_error(
        "Can't calculate complements for matrix with size < 2");
  }
  S21Matrix result(rows_, cols_);
  ComplementsHelp_(result);
  return result;
}

// Helper function for finding the matrix of cofactors
void S21Matrix::ComplementsHelp_(S21Matrix &complements) const {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      S21Matrix minor(rows_ - 1, cols_ - 1);
      FindMinor_(minor, i, j);
      complements.matrix_[i][j] = pow(-1, (i + j)) * minor.Determinant();
      minor.ClearMatrix_();
    }
  }
}

// Finds the minor of the matrix in the specified row and column
void S21Matrix::FindMinor_(S21Matrix &minor, int row, int col) const noexcept {
  int RowCounter = 0, ColCounter = 0;
  for (int i = 0; i < rows_; i++) {
    if (i != row) {
      ColCounter = 0;
      for (int j = 0; j < cols_; j++) {
        if (j != col) {
          minor.matrix_[RowCounter][ColCounter] = matrix_[i][j];
          ColCounter++;
        }
      }
      RowCounter++;
    }
  }
}

// Finds the determinant of the matrix and returns it
double S21Matrix::Determinant() const {
  if (rows_ != cols_) {
    throw std::logic_error("The matrix is not square");
  }
  return DetHelp_();
}

// Recursive function for calculating the determinant of the matrix, returns the
// determinant value
double S21Matrix::DetHelp_() const {
  double total = 0;
  if (rows_ == 1) {
    total = matrix_[0][0];
  } else {
    for (int j = 0; j < cols_; j++) {
      S21Matrix minor(rows_ - 1, cols_ - 1);
      FindMinor_(minor, 0, j);
      total += matrix_[0][j] * pow(-1, j) * minor.DetHelp_();
      minor.ClearMatrix_();
    }
  }
  return total;
}

// Creates the inverse matrix of the current matrix and returns it
S21Matrix S21Matrix::InverseMatrix() const {
  const double determinant = Determinant();
  // 1.0e-07 is 10 * 10 ^ (-7)
  if (fabs(determinant) <= 1.0e-7) {
    throw std::logic_error("Matrix determinant can't be 0");
  }
  S21Matrix inversed = CalcComplements();
  inversed = inversed.Transpose();
  inversed *= (1.0 / determinant);
  return inversed;
}

// Returns the sum of the current matrix and the given matrix
S21Matrix S21Matrix::operator+(const S21Matrix &other) {
  S21Matrix result(*this);
  result.SumMatrix(other);
  return result;
}

// Returns the difference between the current matrix and the given matrix
S21Matrix S21Matrix::operator-(const S21Matrix &other) {
  S21Matrix result(*this);
  result.SubMatrix(other);
  return result;
}

// Returns the product of the current matrix and the given matrix
S21Matrix S21Matrix::operator*(const S21Matrix &other) {
  S21Matrix result(*this);
  result.MulMatrix(other);
  return result;
}

// Returns the product of the current matrix and a number
S21Matrix S21Matrix::operator*(const double mul) {
  S21Matrix result(*this);
  result.MulNumber(mul);
  return result;
}

// Checks if the matrices are equal
bool S21Matrix::operator==(const S21Matrix &other) const noexcept {
  return EqMatrix(other);
}

// Copy assignment operator
S21Matrix &S21Matrix::operator=(const S21Matrix &other) {
  if (this == &other) {
    return *this;
  }
  ClearMatrix_();
  rows_ = other.rows_;
  cols_ = other.cols_;
  CopyMatrix_(other);
  return *this;
}

// Move assignment operator
S21Matrix &S21Matrix::operator=(S21Matrix &&other) {
  if (this == &other) {
    return *this;
  }
  rows_ = std::exchange(other.rows_, 0);
  cols_ = std::exchange(other.cols_, 0);
  matrix_ = std::exchange(other.matrix_, nullptr);
  return *this;
}

// Adds the given matrix to the current matrix and returns it
S21Matrix S21Matrix::operator+=(const S21Matrix &other) {
  SumMatrix(other);
  return *this;
}

// Subtracts the given matrix from the current matrix and returns it
S21Matrix S21Matrix::operator-=(const S21Matrix &other) {
  SubMatrix(other);
  return *this;
}

// Multiplies the current matrix by the given matrix and returns it
S21Matrix S21Matrix::operator*=(const S21Matrix &other) {
  MulMatrix(other);
  return *this;
}

// Multiplies the current matrix by a number and returns it
S21Matrix S21Matrix::operator*=(const double mul) {
  MulNumber(mul);
  return *this;
}

// Returns a pointer to the value of the matrix at the specified row and column
double &S21Matrix::operator()(int row, int col) {
  if (row < 0) {
    throw std::out_of_range("Row can't be less than zero");
  } else if (col < 0) {
    throw std::out_of_range("Column can't be less than zero");
  } else if (row >= rows_) {
    throw std::out_of_range("Row doesn't exist");
  } else if (col >= cols_) {
    throw std::out_of_range("Column doesn't exist");
  }
  return matrix_[row][col];
}

// Returns a pointer to the value of the matrix at the specified row and column
double &S21Matrix::operator()(int row, int col) const {
  if (row < 0) {
    throw std::out_of_range("Row can't be less than zero");
  } else if (col < 0) {
    throw std::out_of_range("Column can't be less than zero");
  } else if (row >= rows_) {
    throw std::out_of_range("Row doesn't exist");
  } else if (col >= cols_) {
    throw std::out_of_range("Column doesn't exist");
  }
  return matrix_[row][col];
}
