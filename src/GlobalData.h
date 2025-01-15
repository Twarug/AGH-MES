#pragma once

#include "Core.h"

namespace mes {

struct GlobalData {
    f32 simulationTime;
    f32 simulationStepTime;
    f32 conductivity;
    f32 alpha;
    f32 Tot;

    f32 initialTemp;
    f32 density;
    f32 specificHeat;

    u64 nodesX, nodesY;

    f32 gridWidth;
    f32 gridHeight;

    static GlobalData readFromFile(const std::string_view& filename);
};

}