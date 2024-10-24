#include <print>

#include "GlobalData.h"
#include "Grid.h"
#include "Integration.h"

using namespace mes;

int main() {
    GlobalData data = GlobalData::readFromFile("data.yaml");

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

    return 0;
}
