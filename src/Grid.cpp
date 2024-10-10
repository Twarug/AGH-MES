#include "Grid.h"

#include <print>

namespace mes {

void Grid::print() const {
    std::println("------------- Nodes --------------");
    i32 i = 1;
    for (const Point& point : points)
        std::println("{}. ({} {})", i++, point.x, point.y);

    std::println("------------- Elements --------------");
    for (const Element& element : elements) {
        std::println("Element: {}", element.index + 1);
        for (u64 index : element.indices)
            std::print("    {}", index + 1);
        std::println("");
    }
}

Grid Grid::generate(f32 width, f32 height, u64 nW, u64 nH) {
    Grid grid {};

    f32 deltaW = width / (nW-1);
    f32 deltaH = height / (nH-1);
    grid.points.reserve(nW * nH);
    for (f32 x = 0; x <= width; x += deltaW)
        for (f32 y = 0; y <= height; y += deltaH) {
            grid.points.push_back(Point{ x, y });
        }

    // Elements Generation
    grid.elements.reserve((nW-1) * (nH - 1));
    for (u64 w = 0; w < nW - 1; w++)
        for (u64 h = 0; h < nH - 1; h++) {
            u64 index = h + w * nW;
            u64 basePointIndex = index + w;
            grid.elements.push_back(Element{
                index, { basePointIndex, basePointIndex + nH, basePointIndex + nH + 1, basePointIndex + 1 }
            });
        }

    return grid;
}

}