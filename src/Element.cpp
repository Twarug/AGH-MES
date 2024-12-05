#include "Element.h"

#include <print>

#include "Surface.h"
#include "Gauss/Quadratures.h"
#include "jacobian/Jacobian.h"

namespace mes {

Matrix& Element::calculateHlocal(const Grid& grid, const GlobalData& data, i32 N) {
    hLocal = std::move(Matrix(indices.size(), indices.size()));
    auto& quadrature = Quadrature::get(N);

    auto jacobians = calculateJacobian(grid, index, N);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            int pcIndex = i * N + j;
            Matrix& J = jacobians[pcIndex];
            f32 detJ = J.determinant();

            f32 ksi = quadrature.xs[i];
            f32 eta = quadrature.xs[j];
            f32 weight = quadrature.weights[i] * quadrature.weights[j];

            Matrix dNdKsiEta = shapeFunctionDerivatives(ksi, eta);
            Matrix invJ = J.inverse();

            std::vector<f32> dNdX(4), dNdY(4);

            for (int k = 0; k < 4; k++) {
                dNdX[k] = invJ(0, 0) * dNdKsiEta(0, k) + invJ(0, 1) * dNdKsiEta(1, k);
                dNdY[k] = invJ(1, 0) * dNdKsiEta(0, k) + invJ(1, 1) * dNdKsiEta(1, k);
            }

            Matrix h(indices.size(), indices.size());
            for (int m = 0; m < indices.size(); m++) {
                for (int n = 0; n < indices.size(); n++) {
                    h(m, n) += (dNdX[m] * dNdX[n] + dNdY[m] * dNdY[n]) * detJ * weight * data.conductivity;
                }
            }

            hLocal += h;
    }
    finalHlocal = hLocal;

    return hLocal;
}

}
