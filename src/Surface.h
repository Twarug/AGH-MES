#pragma once

#include "Core.h"
#include <vector>
#include <array>

#include "Point.h"

namespace mes {

  struct Surface
  {
    u64 id;
    u64 num_points;
    std::array<const Point*, 2> nodes;
    std::vector<f32> weights;
    std::vector<f32> ksi_values;
    std::vector<f32> eta_values;
    std::vector<std::array<f32, 4>> N;

    void calculateN();
    f32 calculateDetJ() const;
    Matrix calculateLocalHbc(f32 alpha) const;
  };

}
