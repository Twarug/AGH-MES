#include <print>
#include <ranges>

#include "common/Utils.h"
#include "GlobalData.h"
#include "Grid.h"
#include "Gauss/Elimination.h"
#include "gauss/Integration.h"
#include "jacobian/Jacobian.h"

using namespace mes;

void lab1();
void lab2();
void lab3();
void lab4();
void lab5();
void lab6();
void lab7();
void simulation();

int main()
{
    try
    {
        // lab1(); std::println("");
        // lab2(); std::println("");
        // lab3(); std::println("");
        // lab4(); std::println("");
        // lab5(); std::println("");
        // lab6(); std::println("");
        // lab7(); std::println("");
        simulation(); std::println("");
    }
    catch (const std::runtime_error& err) {
        std::println("[Err]: {}", err.what());
    }
}

void lab1() {
    GlobalData data = GlobalData::readFromFile("../data/data.yaml");

    Grid grid = Grid::generate(data);

    grid.print();

    std::println("");

    auto f = [](f32 x, f32 y) {
        return -5*x*x*y + 2 * x*y + 10;
    };

    f32 result = integration2D(f, 2);
    std::println("Result 2: {}", result);

    result = integration2D(f, 3);
    std::println("Result 3: {}", result);
}

void lab2()
{
    Grid grid = Grid();

    grid.points.push_back({0, 0});
    grid.points.push_back({4, 0});
    grid.points.push_back({4, 4});
    grid.points.push_back({0, 5});
    grid.elements.push_back({0, {0, 1, 2, 3}});

    auto jacobians = calculateJacobian(grid, 0);
    auto detJacobian = calculateDetJacobian(jacobians);
    auto inverses = calculateInverseJacobian(jacobians);

    for (auto [i, V] : std::views::enumerate(std::views::zip(jacobians, detJacobian, inverses))) {
        auto [J, det, inv] = V;

        std::println("At point: {}", i);
        std::println("  Jacobian: {}", J);
        std::println("  Determinant: {}", det);
        std::println("  Inverse: {}", inv);
    }
}

void lab3() {

    GlobalData data = GlobalData::readFromFile("../data/hglobal/data.yaml");
    Grid grid = Grid::fromFile("../data/hglobal/grid.yaml");

    constexpr int N = 2;

    Matrix H = Matrix(grid.points.size(), grid.points.size());

    for (auto& element : grid.elements)
    {
        std::println("Element: {}", element.index);

        Matrix Hlocal = element.calculateHlocal(grid, data, N);

        std::println("  Hlocal: {}", Hlocal);

        for (int i = 0; i < Hlocal.rows; i++)
            for (int j = 0; j < Hlocal.cols; j++)
                H(element.indices[i], element.indices[j]) += Hlocal(i, j);
    }

    std::println("H: {}", H);
}

void lab4() {

    GlobalData data = GlobalData::readFromFile("../data/hbc/data.yaml");
    Grid grid = Grid::fromFile("../data/hbc/grid.yaml");

    constexpr int N = 4;

    Matrix H = Matrix(grid.points.size(), grid.points.size());

    for (auto& element : grid.elements)
    {
        std::println("Element: {}", element.index);

        Matrix Hlocal = element.calculateHlocal(grid, data, N);
        // std::println("  Hlocal: {}", Hlocal);

        Matrix HbcLocal = element.calculateHbcLocal(grid, data, N);
        std::println("  HLocla: {}", element.finalHlocal);
        // std::println("  HBcLocla: {}", element.hbcLocal);


        // std::println("finalHlocal: {}\nhLocal: {}\nhbcLocal: {}", element.finalHlocal, element.hLocal, element.hbcLocal);


        for (int i = 0; i < Hlocal.rows; i++)
            for (int j = 0; j < Hlocal.cols; j++)
                H(element.indices[i], element.indices[j]) += element.finalHlocal(i, j);
    }

    std::println("H: {}", H);
}

void lab5() {

    GlobalData data = GlobalData::readFromFile("../data/pvec/data.yaml");
    Grid grid = Grid::fromFile("../data/pvec/grid.yaml");

    constexpr int N = 3;

    std::vector<f32> P = std::vector<f32>(grid.points.size(), 0);

    for (auto& element : grid.elements)
    {
        std::println("Element: {}", element.index + 1);

        std::vector<f32>& PVecLocal = element.calculatePlocal(grid, data, N);
        std::println("  [Plocal]: {}", PVecLocal);

        for (int i = 0; i < PVecLocal.size(); i++)
            P[element.indices[i]] += PVecLocal[i];
    }

    std::println("[P]: {}", P);
}

void lab6() {
    GlobalData data = GlobalData::readFromFile("../data/static/data.yaml");
    Grid grid = Grid::fromFile("../data/static/grid.yaml");

    constexpr int N = 3;

    Matrix H = Matrix(grid.points.size(), grid.points.size());
    std::vector<f32> P = std::vector<f32>(grid.points.size(), 0);

    for (auto& element : grid.elements)
    {
        std::println("Element: {}", element.index + 1);

        element.calculateHlocal(grid, data, N);
        element.calculateHbcLocal(grid, data, N);

        Matrix Hlocal = element.finalHlocal;
        std::println("  Hlocal: {}", Hlocal);

        std::vector<f32>& PVecLocal = element.calculatePlocal(grid, data, N);
        std::println("  [Plocal]: {}", PVecLocal);

        for (int i = 0; i < Hlocal.rows; i++) {
            for (int j = 0; j < Hlocal.cols; j++)
                H(element.indices[i], element.indices[j]) += Hlocal(i, j);
            P[element.indices[i]] += PVecLocal[i];
        }
    }

    std::println("H: {}", H);
    std::println("[P]: {}", P);

    for (auto& p : P)
        p *= -1;

    std::vector<f32> T = gaussianElimination(H, P);

    for (auto& t : T)
        t *= -1;

    std::println("[T]: {}", T);
}

void lab7()
{
    GlobalData data = GlobalData::readFromFile("../data/C/data.yaml");
    Grid grid = Grid::fromFile("../data/C/grid.yaml");

    constexpr int N = 2;


    Matrix C = Matrix(grid.points.size(), grid.points.size());

    for (auto& element : grid.elements)
    {
        Matrix& cLocal =  element.calculateCLocal(grid, data, N);

        for (int i = 0; i < cLocal.rows; i++) {
            for (int j = 0; j < cLocal.cols; j++)
                C(element.indices[i], element.indices[j]) += cLocal(i, j);
        }
    }


    std::println("[C]: {}", C);
}

void simulation()
{
    GlobalData data = GlobalData::readFromFile("../data/data.yaml");
    Grid grid = Grid::fromFile("../data/grid-mix.yaml");
    // Grid grid = Grid::generate(data);

    constexpr int N = 2;

    Matrix H = Matrix(grid.points.size(), grid.points.size());
    std::vector<f32> P = std::vector(grid.points.size(), 0.f);
    Matrix C = Matrix(grid.points.size(), grid.points.size());
    std::vector<f32> T0 = std::vector(grid.points.size(), data.initialTemp);

    for (auto& element : grid.elements) {
        // std::println("Element: {}", element.index + 1);
        element.calculateHlocal(grid, data, N);
        element.calculateHbcLocal(grid, data, N);

        Matrix Hlocal = element.finalHlocal;
        // std::println("  Hlocal: {}", Hlocal);

        std::vector<f32>& PVecLocal = element.calculatePlocal(grid, data, N);
        // std::println("  [Plocal]: {}", PVecLocal);

        Matrix& cLocal =  element.calculateCLocal(grid, data, N);
        // std::println("  [Clocal]: {}", cLocal);

        for (int i = 0; i < element.indices.size(); i++) {
            for (int j = 0; j < element.indices.size(); j++) {
                H(element.indices[i], element.indices[j]) += Hlocal(i, j);
                C(element.indices[i], element.indices[j]) += cLocal(i, j);
            }
            P[element.indices[i]] += PVecLocal[i];
        }
    }

    // std::println("H: {}", H);
    // std::println("[P]: {}", P);
    // std::println("C: {}", C);
    // std::println("T0: {}", T0);


    int i = 0;
    for (float time = 0; time < data.simulationTime; time += data.simulationStepTime)
    {
        // std::println("Interation: {}", i++);
        Matrix Cdt = C * (1.0 / data.simulationStepTime);
        std::vector<f32> Ct0 = Cdt * T0 - P;

        Matrix A = H + Cdt;
        std::vector<f32> b = P + Cdt * T0;

        // std::println("H: {}", A);
        // std::println("[P]: {}", P);

        std::vector<f32> T1  = gaussianElimination(A, b);
        // std::println("T1: {}", T1);

        f32 min = std::numeric_limits<f32>::max();
        f32 max = std::numeric_limits<f32>::min();
        for (auto& t : T1) {
            min = std::min(min, t);
            max = std::max(max, t);
        }

        std::println("min: {}, max: {}", min, max);

        T0 = std::move(T1);
    }
}

