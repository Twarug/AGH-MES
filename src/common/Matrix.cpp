#include "Matrix.h"

#include <print>
#include <stdexcept>

namespace mes {
    Matrix::Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        data = new f32[rows * cols];
        for (u64 i = 0; i < rows; ++i) {
            for (u64 j = 0; j < cols; ++j) {
                data[i * cols + j] = other.data[i * cols + j];
            }
        }
    }

    Matrix& Matrix::operator=(const Matrix& other)
    {
        *this = std::move(Matrix(other));
        return *this;
    }

    Matrix::Matrix(Matrix&& other) noexcept
        : rows(std::exchange(other.rows, 0)), cols(std::exchange(other.cols, 0)), data(std::exchange(other.data, nullptr)) {}

    Matrix& Matrix::operator=(Matrix&& other) noexcept {
        delete[] data;
        rows = std::exchange(other.rows, 0);
        cols = std::exchange(other.cols, 0);
        data = std::exchange(other.data, nullptr);
        return *this;
    }

    Matrix Matrix::inverse() const {
        if (rows != cols)
            throw std::invalid_argument("Matrix must be square to calculate inverse.");

        u64 n = rows; // Since the matrix is square, rows == cols
        f32 det = determinant();

        if (det == 0)
            throw std::runtime_error("Matrix is singular and cannot be inverted.");

        Matrix result(n, n);

        // Compute cofactors and adjugate
        for (u64 i = 0; i < n; ++i) {
            for (u64 j = 0; j < n; ++j) {
                // Cofactor of element (i, j)
                Matrix sub = subMatrix(i, j);
                f32 cofactor = sub.determinant();
                if ((i + j) % 2 != 0) {
                    cofactor = -cofactor; // Adjust sign for cofactor
                }
                result(j, i) = cofactor / det; // Transpose in the same step (adjugate)
            }
        }

        return result;
    }

    Matrix Matrix::subMatrix(u64 i, u64 j) const
    {
        Matrix sub(rows - 1, cols - 1);
        for (u64 k = 0; k < rows; ++k) {
            for (u64 l = 0; l < cols; ++l) {
                if (k < i && l < j) {
                    sub(k, l) = data[k * cols + l];
                } else if (k < i && l > j) {
                    sub(k, l - 1) = data[k * cols + l];
                } else if (k > i && l < j) {
                    sub(k - 1, l) = data[k * cols + l];
                } else if (k > i && l > j) {
                    sub(k - 1, l - 1) = data[k * cols + l];
                }
            }
        }

        return sub;
    }

    void Matrix::swapRows(u64 i, u64 j) const
    {
        for (int k = 0; k < cols; k++) {
            std::swap(data[i * cols + k], data[j * cols + k]);
        }
    }

    Matrix Matrix::operator*(f32 scalar) const
    {
        Matrix value(rows, cols);

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                value(i, j) = this->operator()(i, j) * scalar;

        return value;
    }

    Matrix& Matrix::operator*=(f32 scalar)
    {
        for (u64 i = 0; i < rows; ++i) {
            for (u64 j = 0; j < cols; ++j) {
                this->operator()(i, j) *= scalar;
            }
        }

        return *this;
    }

    Matrix& Matrix::operator+=(const Matrix& other)
    {
        if (cols != other.cols || rows != other.rows)
            throw std::runtime_error("Matrices must have the same dimensions");

        for (u64 i = 0; i < rows; ++i) {
            for (u64 j = 0; j < cols; ++j) {
                data[i * cols + j] += other.data[i * cols + j];
            }
        }

        return *this;
    }

    f32 Matrix::determinant() const {
        if (cols != rows)
            throw std::runtime_error("Matrix is not square");

        u64 n = cols;
        if (n == 1) {
            return (*this)(0, 0);
        }

        if (n == 2) {
            return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
        }

        f32 det = 0;
        for (u64 i = 0; i < n; ++i) {
            Matrix submatrix = subMatrix(0, i);
            f32 cofactor = i % 2 == 0 ? (*this)(0, i) : -(*this)(0, i);
            det += cofactor * submatrix.determinant();
        }

        return det;
    }

}
