#ifndef S21_MATRIX_OOP_H
#define S21_MATRIX_OOP_H

#include <cmath>
#include <stdexcept>

class S21Matrix {
 public:
  /* ===================== Constructors and destructors ===================== */
  S21Matrix() noexcept;
  S21Matrix(int rows, int cols);
  ~S21Matrix();
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other) noexcept;

  /* ======================== Accessors and mutatos ========================= */
  int GetRows() const noexcept;
  int GetCols() const noexcept;
  void SetRows(int rows);
  void SetCols(int cols);

  /* ============================== Functions =============================== */
  bool EqMatrix(const S21Matrix& other) const noexcept;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose() const noexcept;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix() const;

  /* ============================== Operators =============================== */
  S21Matrix operator+(const S21Matrix& other);
  S21Matrix operator-(const S21Matrix& other);
  S21Matrix operator*(const S21Matrix& other);
  S21Matrix operator*(const double mul);
  bool operator==(const S21Matrix& other) const noexcept;
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix& operator=(S21Matrix&& other);
  S21Matrix operator+=(const S21Matrix& other);
  S21Matrix operator-=(const S21Matrix& other);
  S21Matrix operator*=(const S21Matrix& other);
  S21Matrix operator*=(const double mul);
  double& operator()(int row, int col);

 private:
  /* ============================= Attributes =============================== */
  int rows_, cols_;
  double** matrix_;

  /* ============================== Methods ================================= */
  void InitMatrix_();
  void CopyMatrix_(const S21Matrix& other);
  void ClearMatrix_() noexcept;
  void ComplementsHelp_(S21Matrix& complements) const;
  void FindMinor_(S21Matrix& minor, int row, int col) const noexcept;
  double DetHelp_() const noexcept;
};

#endif  // S21_MATRIX_OOP_H
