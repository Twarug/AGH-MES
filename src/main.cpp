#include "GlobalData.h"
#include "Grid.h"

using namespace mes;

int main() {
    GlobalData data = GlobalData::readFromFile("data.yaml");

    Grid grid = Grid::generate(data);

    grid.print();

    return 0;
}
