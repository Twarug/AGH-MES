#pragma once

#include <functional>

#include "Core.h"

namespace mes {

    f32 integration(const std::function<f32(f32)>& f, u32 n = 2);

    f32 integration2D(const std::function<f32(f32, f32)>& f, u32 n = 2);

}