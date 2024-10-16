#pragma once

#include <functional>

#include "Core.h"

namespace mes {

    f32 integration(std::function<f32(f32)> f, u32 n = 2);

    f32 integration2D(std::function<f32(f32, f32)> f, u32 n = 2);

}