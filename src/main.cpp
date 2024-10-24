#include <print>
#include <ranges>

#include "GlobalData.h"
#include "Grid.h"
#include "gauss/Integration.h"
#include "jacobian/Jacobian.h"

using namespace mes;

int main() {
    GlobalData data = GlobalData::readFromFile("data.yaml");

    // Grid grid = Grid::generate(data);
    //
    // grid.print();

    // std::println("");
    //
    // auto f = [](f32 x, f32 y) {
    //     return -5*x*x*y + 2 * x*y + 10;
    // };
    //
    // f32 result = integration2D(f, 2);
    // std::println("Result 2: {}", result);
    //
    // result = integration2D(f, 3);
    // std::println("Result 3: {}", result);

    std::println("");

    Grid grid = Grid();

    grid.points.push_back({0, 0});
    grid.points.push_back({4, 0});
    grid.points.push_back({4, 4});
    grid.points.push_back({0, 4});
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

    return 0;
}
