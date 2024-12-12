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

std::vector<Surface> Element::getSurfaces(const Grid& grid, i32 N) const {
    std::vector<Surface> surfaces;
    const Quadrature& quad = Quadrature::get(N);

    for (u64 i = 0; i < 4; i++)
    {
        const auto& point1 = grid.points[indices[i]];
        const auto& point2 = grid.points[indices[(i + 1) % 4]];

        if (point1.BC && point2.BC)
        {
            std::vector<f32> ksi_values, eta_values;

            if (i % 2 == 0) {
                ksi_values = quad.xs;
                eta_values = i == 0 ? std::vector(N, -1.0f) : std::vector(N, 1.0f);
            } else {
                eta_values = quad.xs;
                ksi_values = i == 1 ? std::vector(N, 1.0f) : std::vector(N, -1.0f);
            }

            surfaces.emplace_back(i, N, std::array{&point1, &point2}, quad.weights, ksi_values, eta_values);
        }
    }
    return surfaces;
}

Matrix& Element::calculateHbcLocal(const Grid& grid, const GlobalData& data, i32 N) {
    if (finalHlocal.cols != indices.size() || finalHlocal.rows != indices.size())
        throw std::runtime_error(std::format("finalHlocal is not a {0}x{0} matrix", indices.size()));

    hbcLocal = std::move(Matrix(indices.size(), indices.size()));

    std::vector<Surface> surfaces = getSurfaces(grid, N);

    for (auto& surface : surfaces)
    {
        Matrix hbcSurf = surface.calculateLocalHbc(data.alpha);

        hbcLocal += hbcSurf;
    }

    finalHlocal += hbcLocal;

    return hbcLocal;
}

std::vector<f32>& Element::calculatePlocal(const Grid& grid, const GlobalData& data, i32 N)
{
    Plocal.resize(4, 0);

    std::vector<Surface> surfaces = getSurfaces(grid, N);

    for (auto& surface : surfaces) {
        std::vector<f32> Psurf = surface.calculatePlocal(data.alpha, data.Tot);
        for (int i = 0; i < 4; i++) {
            Plocal[i] += Psurf[i];
        }
    }

    return Plocal;
}

}
