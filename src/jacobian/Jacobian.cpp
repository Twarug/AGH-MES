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

        const Quadrature& quadrature = Quadrature::get(N);

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {
                Matrix J(2, 2);
                Matrix dNdKsiEta = shapeFunctionDerivatives(quadrature.xs[i], quadrature.xs[j]);

                // Compute Jacobian matrix at each Gauss point
                for (u64 k = 0; k < 4; ++k) {
                    u64 node_id = grid.elements[elementIndex].indices[k];
                    f32 xk = grid.points[node_id].x;
                    f32 yk = grid.points[node_id].y;

                    J(0, 0) += dNdKsiEta(0, k) * xk;  // ∂x/∂ξ
                    J(0, 1) += dNdKsiEta(0, k) * yk;  // ∂y/∂ξ
                    J(1, 0) += dNdKsiEta(1, k) * xk;  // ∂x/∂η
                    J(1, 1) += dNdKsiEta(1, k) * yk;  // ∂y/∂η
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
