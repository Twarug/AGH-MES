#pragma once
#include <array>

#include "Core.h"
#include "GlobalData.h"

namespace mes {
struct Grid;

struct Element {
    u64 index;
    std::array<u64, 4> indices;

    Element(u64 index, std::array<u64, 4> indices) : index(index), indices(indices) {}

    Matrix hLocal;
    Matrix finalHlocal;

    Matrix& calculateHlocal(const Grid& grid, const GlobalData& data, i32 N);
};

}
