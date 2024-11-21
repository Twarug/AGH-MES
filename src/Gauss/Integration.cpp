#include "Integration.h"
#include "Quadratures.h"

namespace mes {

    f32 integration(const std::function<f32(f32)>& f, u32 n) {
        const Quadrature& q = Quadrature::get(n - 1);

        f32 result = 0.0;
        for (int i = 0; i < n; ++i) {
            result += q.weights[i] * f(q.xs[i]);
        }

        return result;
    }


    f32 integration2D(const std::function<f32(f32, f32)>& f, u32 n) {
        auto integrateX = [&](f32 y) {
            return integration([f, y](f32 x) {
                return f(x, y);
            }, n);
        };

        return integration(integrateX, n);
    }

}