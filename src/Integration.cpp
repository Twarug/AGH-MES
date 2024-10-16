#include "Integration.h"

#include <vector>

namespace mes {

    struct Quadrature {
        std::vector<f32> xs;
        std::vector<f32> weights;
    };

    static std::vector<Quadrature> quadratureGL = {
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

    f32 integration(std::function<f32(f32)> f, u32 n)
    {
        n--;
        Quadrature& q = quadratureGL[n];

        double result = 0.0;
        for (int i = 0; i < n + 1; ++i) {
            result += q.weights[i] * f(q.xs[i]);
        }

        return result;
    }


    f32 integration2D(std::function<f32(f32, f32)> f, u32 n) {
        auto integrateX = [&](f32 y) {
            return integration([f, y](f32 x) {
                return f(x, y);
            }, n);
        };

        return integration(integrateX, n);
    }

}