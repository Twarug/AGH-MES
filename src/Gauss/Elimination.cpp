#include "Elimination.h"

namespace mes {

    std::vector<f32> gaussianElimination(Matrix A, std::vector<f32> b) {
        if (A.rows != A.cols || A.rows != b.size())
            throw std::runtime_error("Matrix and vector sizes do not match");

        int n = A.rows;

        // Forward Elimination
        for (int i = 0; i < n; i++) {
            // Find the pivot element
            int maxIndex = i;
            for (int j = i + 1; j < n; j++) {
                if (std::abs(A(j, i)) > std::abs(A(maxIndex, i))) {
                    maxIndex = j;
                }
            }

            // Swap rows if needed
            if (A(maxIndex, i) == 0) {
                throw std::runtime_error("Matrix is singular");
            }

            if (maxIndex != i) {
                std::swap(b[i], b[maxIndex]);
                A.swapRows(i, maxIndex);
            }

            // Eliminate the entries below the pivot
            for (int j = i + 1; j < n; j++) {
                f32 factor = A(j, i) / A(i, i);
                for (int k = i; k < n; k++) {
                    A(j, k) -= factor * A(i, k);
                }
                b[j] -= factor * b[i];
            }
        }

        // Back Substitution
        std::vector<f32> x(n, 0);
        for (int i = n - 1; i >= 0; i--) {
            x[i] = b[i];
            for (int j = i + 1; j < n; j++) {
                x[i] -= A(i, j) * x[j];
            }
            x[i] /= A(i, i);
        }

        return x;
    }

}