#pragma once

#include "Core.h"
#include <vector>


namespace mes {

    struct Quadrature {
        std::vector<f32> xs;
        std::vector<f32> weights;

        static const Quadrature& get(u32 n)
        {
            if (n > quadratures.size())
                throw std::runtime_error("Quadrature of unsupported size.");

            return quadratures[n - 1];
        }

    private:
        static std::vector<Quadrature> quadratures;
    };

}