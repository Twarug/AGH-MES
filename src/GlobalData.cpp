#include "GlobalData.h"

#include "yaml-cpp/yaml.h"

namespace mes {

GlobalData GlobalData::readFromFile(const std::string_view& filename) {
    YAML::Node config = YAML::LoadFile(std::string(filename));

    GlobalData data;

    data.simulationTime = config["simulationTime"].as<f32>(-1.f);
    data.simulationStepTime = config["simulationStepTime"].as<f32>(-1.f);
    data.conductivity = config["conductivity"].as<f32>(-1.f);
    data.alpha = config["alpha"].as<f32>(-1.f);
    data.Tot = config["Tot"].as<f32>(-1.f);

    data.InitialTemp100 = config["InitialTemp100"].as<f32>(-1.f);
    data.Density = config["Density"].as<f32>(-1.f);
    data.SpecificHeat = config["SpecificHeat"].as<f32>(-1.f);

    data.nodesX = config["nodesX"].as<u64>(-1);
    data.nodesY = config["nodesY"].as<u64>(-1);

    data.gridWidth = config["gridWidth"].as<f32>(-1.f);
    data.gridHeight = config["gridHeight"].as<f32>(-1.f);

    return data;
}

}