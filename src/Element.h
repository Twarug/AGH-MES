#pragma once
#include <array>

#include "Core.h"

namespace mes {

struct Element {
    u64 index;
    std::array<u64, 4> indices;
};

}
