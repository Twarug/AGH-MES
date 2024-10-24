#include "Jacobian.h"

#include "Grid.h"

#include <iostream>
#include <print>
#include <stdexcept>
#include <Gauss/Quadratures.h>

namespace mes {

    std::vector<Matrix> calculateJacobian(const Grid& grid, u64 elementIndex, u64 N) {
        std::vector<Matrix> jacobians;
        jacobians.reserve(N * N);

        std::vector<f32> ksi;
        std::vector<f32> eta;

        std::vector<std::vector<f32>> dKsi;
        std::vector<std::vector<f32>> dEta;

        auto& quadrature = quadratureGL[N];

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                ksi.push_back(quadrature.xs[i]);
                eta.push_back(quadrature.xs[j]);

                // Derivatives of shape functions with respect to Ksi and Eta
                dKsi.push_back({
                    -0.25f * (1 - eta.back()),  0.25f * (1 - eta.back()),
                     0.25f * (1 + eta.back()), -0.25f * (1 + eta.back())
                });
                dEta.push_back({
                    -0.25f * (1 - ksi.back()), -0.25f * (1 + ksi.back()),
                     0.25f * (1 + ksi.back()),  0.25f * (1 - ksi.back())
                });
            }
        }


        for (size_t i = 0; i < ksi.size(); ++i) {
            Matrix J(2, 2);

            // Compute Jacobian matrix at each Gauss point
            for (int k = 0; k < 4; ++k) {
                int node_id = grid.elements[elementIndex].indices[k];
                float xk = grid.points[node_id].x;
                float yk = grid.points[node_id].y;

                J(0, 0) += dKsi[i][k] * xk;  // ∂x/∂ξ
                J(0, 1) += dKsi[i][k] * yk;  // ∂y/∂ξ
                J(1, 0) += dEta[i][k] * xk;  // ∂x/∂η
                J(1, 1) += dEta[i][k] * yk;  // ∂y/∂η
            }

            jacobians.push_back(J);
        }

        return jacobians;
    }

    std::vector<f32> calculateDetJacobian(const std::vector<Matrix>& jacobians) {
        std::vector<f32> dets;

        // Compute the determinant of each Jacobian matrix
        for (const auto& J : jacobians) {
            dets.push_back(J.determinant());
        }

        return dets;
    }

    std::vector<Matrix> calculateInverseJacobian(const std::vector<Matrix>& jacobians) {
        std::vector<Matrix> inverses;

        // Compute the inverse of each Jacobian matrix
        for (const auto& J : jacobians) {
            try {
                inverses.push_back(J.inverse());
            } catch (const std::runtime_error& e) {
                // Handle singular matrix case
                std::println(std::cerr, "Jacobian {}", e.what());
                inverses.push_back(Matrix(2, 2));
            }
        }

        return inverses;
    }

}
