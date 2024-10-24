#pragma once

#include <functional>

#include "Core.h"

namespace mes {
    struct Quadrature {
        std::vector<f32> xs;
        std::vector<f32> weights;
    };

    extern std::vector<Quadrature> quadratureGL;

    f32 integration(std::function<f32(f32)> f, u32 n = 2);

    f32 integration2D(std::function<f32(f32, f32)> f, u32 n = 2);

}