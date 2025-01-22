#include "Grid.h"

#include <fstream>
#include <print>
#include <yaml-cpp/yaml.h>

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

Grid Grid::generate(const GlobalData& globalData) {
    Grid grid {};

    f32 width = globalData.gridWidth;
    f32 height = globalData.gridHeight;
    u64 nW = globalData.nodesX;
    u64 nH = globalData.nodesY;

    f32 deltaW = width / (nW-1);
    f32 deltaH = height / (nH-1);
    grid.points.reserve(nW * nH);
    for (int x = 0; x < nW; x++)
        for (int y = 0; y < nH; y++) {
            grid.points.push_back(Point{ x * deltaW, y * deltaH, x == 0 || x == nW - 1 || y == 0 || y == nH - 1 });
        }

    // Elements Generation
    grid.elements.reserve((nW-1) * (nH - 1));
    for (u64 w = 0; w < nW - 1; w++)
        for (u64 h = 0; h < nH - 1; h++) {
            u64 index = h + w * (nW - 1);
            u64 basePointIndex = index + w;
            grid.elements.push_back(Element{
                index, { basePointIndex, basePointIndex + nH, basePointIndex + nH + 1, basePointIndex + 1 }
            });
        }

    return grid;
}

Grid Grid::fromFile(const std::filesystem::path& filename)
{
    std::ifstream file(filename);
    YAML::Node config = YAML::Load(file);

    Grid data{};

    YAML::Node nodes = config["nodes"];
    for (YAML::Node node : nodes) {
        f32 x = node["x"].as<f32>();
        f32 y = node["y"].as<f32>();
        bool BC = node["BC"].as<bool>(false);
        data.points.push_back({ x, y, BC });
    }

    u64 i = 0;
    for (YAML::Node element : config["elements"]) {
        std::array<u64, 4> indices{};
        u64 j = 0;
        for (YAML::Node index : element["indices"])
            indices[j++] = index.as<u64>();
        data.elements.push_back({ i++, indices });
    }

    return data;
}

}
