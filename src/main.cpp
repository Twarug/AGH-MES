#include <print>
#include <ranges>

#include "GlobalData.h"
#include "Grid.h"
#include "gauss/Integration.h"
#include "Gauss/Quadratures.h"
#include "jacobian/Jacobian.h"

using namespace mes;

void lab1();
void lab2();
void lab3();

int main()
{
    try
    {
        // lab1(); std::println("");
        // lab2(); std::println("");
        lab3(); std::println("");
    }
    catch (const std::runtime_error& err) {
        std::println("[Err]: {}", err.what());
    }
}

void lab1() {
    GlobalData data = GlobalData::readFromFile("../data/data.yaml");

    Grid grid = Grid::generate(data);

    grid.print();

    std::println("");

    auto f = [](f32 x, f32 y) {
        return -5*x*x*y + 2 * x*y + 10;
    };

    f32 result = integration2D(f, 2);
    std::println("Result 2: {}", result);

    result = integration2D(f, 3);
    std::println("Result 3: {}", result);
}

void lab2()
{
    Grid grid = Grid();

    grid.points.push_back({0, 0});
    grid.points.push_back({4, 0});
    grid.points.push_back({4, 4});
    grid.points.push_back({0, 5});
    grid.elements.push_back({0, {0, 1, 2, 3}});

    auto jacobians = calculateJacobian(grid, 0);
    auto detJacobian = calculateDetJacobian(jacobians);
    auto inverses = calculateInverseJacobian(jacobians);

    for (auto [i, V] : std::views::enumerate(std::views::zip(jacobians, detJacobian, inverses))) {
        auto [J, det, inv] = V;

        std::println("At point: {}", i);
        std::println("  Jacobian: {}", J);
        std::println("  Determinant: {}", det);
        std::println("  Inverse: {}", inv);
    }
}

void lab3() {

    GlobalData data = GlobalData::readFromFile("../data/hglobal/data.yaml");
    Matrix dNdKsiEta(2, 4);
    Grid grid = Grid::fromFile("../data/hglobal/grid.yaml");

    int N = 2;

    Matrix H = Matrix(grid.points.size(), grid.points.size());

    for (auto& element : grid.elements)
    {
        std::println("Element: {}", element.index);

        Matrix Hlocal(4, 4);
        auto& quadrature = quadratureGL[N - 1];

        auto jacobians = calculateJacobian(grid, element.index, N);

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

                std::println("  pc{}", pcIndex + 1);
                std::println("    J: {}", J);
                std::println("    detJ: {}", detJ);
                std::println("    invJ: {}", invJ);
                std::println("    ksi: {}", ksi);
                std::println("    eta: {}", eta);
                std::println("    dNdKsiEta: {}", dNdKsiEta);

                std::vector<f32> dNdX(4), dNdY(4);

                for (int k = 0; k < 4; k++) {
                    dNdX[k] = invJ(0, 0) * dNdKsiEta(0, k) + invJ(0, 1) * dNdKsiEta(1, k);
                    dNdY[k] = invJ(1, 0) * dNdKsiEta(0, k) + invJ(1, 1) * dNdKsiEta(1, k);
                }

                std::println("    dNdX: {}", dNdX);
                std::println("    dNdY: {}", dNdY);

                Matrix H(4, 4);
                for (int m = 0; m < 4; m++) {
                    for (int n = 0; n < 4; n++) {
                        H(m, n) += (dNdX[m] * dNdX[n] + dNdY[m] * dNdY[n]) * detJ * weight * data.conductivity;
                    }
                }

                // std::println("    H: {}", H);
                Hlocal += H;
            }

        std::println("  Hlocal: {}", Hlocal);

        for (int i = 0; i < Hlocal.rows; i++)
            for (int j = 0; j < Hlocal.cols; j++)
                H(element.indices[i], element.indices[j]) += Hlocal(i, j);
    }

    std::println("H: {}", H);
}
