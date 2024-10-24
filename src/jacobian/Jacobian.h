#pragma once

#include "Core.h"

#include "Grid.h"
#include <vector>

namespace mes {

    std::vector<Matrix> calculateJacobian(const Grid& grid, u64 elementIndex, u64 N = 2);
    std::vector<f32> calculateDetJacobian(const std::vector<Matrix>& jacobians);
    std::vector<Matrix> calculateInverseJacobian(const std::vector<Matrix>& jacobians);

}
