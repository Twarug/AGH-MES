#include "GlobalData.h"
#include "Grid.h"

using namespace mes;

int main() {
    GlobalData data = GlobalData::readFromFile("data.yaml");

    const f32 W = 4.0f;
    const f32 H = 4.0f;
    const u64 nW = 4;
    const u64 nH = 6;

    Grid grid = Grid::generate(W, H, nW, nH);

    grid.print();

    return 0;
}
