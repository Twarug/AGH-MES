#include "Integration.h"
#include "Quadratures.h"

namespace mes {

    f32 integration(std::function<f32(f32)> f, u32 n) {
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