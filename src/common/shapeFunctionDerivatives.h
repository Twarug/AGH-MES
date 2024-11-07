#pragma once
#include "Numbers.h"
#include "Matrix.h"

namespace mes {

static Matrix shapeFunctionDerivatives(double ksi, double eta) {
    // Macierz 4x2 przechowująca dN/dksi i dN/deta dla 4 funkcji kształtu
    Matrix dNdKsiEta(2, 4);

    // Pochodne po ksi
    dNdKsiEta(0, 0) = -0.25 * (1 - eta);
    dNdKsiEta(0, 1) =  0.25 * (1 - eta);
    dNdKsiEta(0, 2) =  0.25 * (1 + eta);
    dNdKsiEta(0, 3) = -0.25 * (1 + eta);

    // Pochodne po eta
    dNdKsiEta(1, 0) = -0.25 * (1 - ksi);
    dNdKsiEta(1, 1) = -0.25 * (1 + ksi);
    dNdKsiEta(1, 2) =  0.25 * (1 + ksi);
    dNdKsiEta(1, 3) =  0.25 * (1 - ksi);

    return dNdKsiEta;
}

}
