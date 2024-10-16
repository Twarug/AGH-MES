#pragma once
#include <vector>

#include "Element.h"
#include "GlobalData.h"
#include "Point.h"

namespace mes {

struct Grid {
    std::vector<Point> points;
    std::vector<Element> elements;

    void print() const;

    static Grid generate(GlobalData globalData);
};

}
