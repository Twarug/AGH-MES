#pragma once

#include "Core.h"
#include <vector>


namespace mes {

    struct Quadrature {
        std::vector<f32> xs;
        std::vector<f32> weights;
    };

    inline static std::vector<Quadrature> quadratureGL = {
        {
            { 0.0 },
            { 2.0 }
        },
        {
            { -0.5773502691896257, 0.5773502691896257 },
            { 1.0, 1.0 }
        },
        {
            { -0.7745966692414834, 0.0, 0.7745966692414834 },
            { 0.5555555555555556, 0.8888888888888888, 0.5555555555555556 }
        },
    };

}