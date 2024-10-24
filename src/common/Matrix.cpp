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

    Matrix Matrix::inverse() const {
        f32 det = determinant();
        if (det == 0)
            throw std::runtime_error("Matrix is singular");

        Matrix adj = Matrix(rows, cols);

        for (u64 i = 0; i < rows; ++i) {
            for (u64 j = 0; j < cols; ++j) {
                Matrix sub = subMatrix(i, j);
                f32 subDet = sub.determinant();

                adj(i, j) = (i + j) % 2 == 0 ? subDet : -subDet;
                adj(i, j) /= det;
            }
        }

        return adj;
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

    f32 Matrix::determinant() const {
        if (cols != rows)
            throw std::runtime_error("Matrix is not square");

        u64 n = cols;
        if (n == 1) {
            return (*this)(0, 0);
        }

        if (n == 2) {
            return (*this)(0, 0) * (*this)(1, 1) - ((*this)(0, 1) * (*this)(1, 0));
        }

        f32 det = 0;
        for (u64 i = 0; i < n; ++i) {
            Matrix submatrix = subMatrix(0, i);
            f32 cofactor = (i % 2 == 0) ? (*this)(0, i) : -(*this)(0, i);
            det += cofactor * submatrix.determinant();
        }

        return det;
    }

}