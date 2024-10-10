#pragma once
#include <vector>

#include "Element.h"
#include "Point.h"

namespace mes {

struct Grid {
    std::vector<Point> points;
    std::vector<Element> elements;

    static Grid generate(f32 width, f32 height, u64 nW, u64 nH);
};

}
