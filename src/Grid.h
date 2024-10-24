#pragma once
#include <filesystem>
#include <vector>

#include "Element.h"
#include "GlobalData.h"
#include "Point.h"

namespace mes {

struct Grid {
    std::vector<Point> points;
    std::vector<Element> elements;

    void print() const;

    static Grid generate(const GlobalData& globalData);
    static Grid fromFile(const std::filesystem::path& filename);
};

}
