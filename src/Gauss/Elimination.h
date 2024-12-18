#pragma once

#include <vector>
#include <array>

#include "Core.h"

namespace mes {

  std::vector<f32> gaussianElimination(Matrix A, std::vector<f32> b);

}