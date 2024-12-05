#include "Surface.h"

#include <print>
#include <array>

namespace mes {
void Surface::calculateN() {
    for (int i = 0; i < num_points; i++) {
        f32 ksi = ksi_values[i];
        f32 eta = eta_values[i];

        N.push_back({
            0.25f * (1 - ksi) * (1 - eta),
            0.25f * (1 + ksi) * (1 - eta),
            0.25f * (1 + ksi) * (1 + eta),
            0.25f * (1 - ksi) * (1 + eta)
        });
    }
}

f32 Surface::calculateDetJ() const {
    auto node1 = nodes[0];
    auto node2 = nodes[1];
    double dx = node2->x - node1->x;
    double dy = node2->y - node1->y;
    return std::sqrt(dx * dx + dy * dy) / 2.0f;
}

Matrix Surface::calculateLocalHbc(f32 alpha) const {
    Matrix Hbc_local(4, 4);
    for (int i = 0; i < num_points; ++i) {
        const auto& Ni = N[i];
        Matrix HbcPc(4, 4);

        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                HbcPc(j, k) = Ni[j] * Ni[k] * weights[i] * alpha;
            }
        }

        // Debugowanie
        if (false) {
            std::println("Ksi: {} Eta: {} N1: {} N2: {} N3: {} N4: {}", ksi_values[i], eta_values[i], Ni[0], Ni[1], Ni[2], Ni[3]);
            std::println("pc{}{}: {}", id + 1, i + 1, HbcPc);
        }

        Hbc_local += HbcPc;
    }
    Hbc_local *= calculateDetJ();

    // std::println("pc{}: {}", id + 1, Hbc_local);

    return Hbc_local;
}

}