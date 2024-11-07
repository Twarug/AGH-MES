#include <print>
#include <ranges>

#include "GlobalData.h"
#include "Grid.h"
#include "gauss/Integration.h"
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

}
