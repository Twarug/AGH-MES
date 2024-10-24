#pragma once

#include "Core.h"
#include <vector>


namespace mes {

    struct Quadrature {
        std::vector<f32> xs;
        std::vector<f32> weights;
    };

    extern std::vector<Quadrature> quadratureGL;

}