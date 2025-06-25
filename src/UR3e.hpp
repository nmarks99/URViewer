#ifndef _UR3E_HPP
#define _UR3E_HPP

#include "raylib.h"
#include "raymath.h"

constexpr Matrix TS1 {
    1.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 1.000000, 0.000000, 0.15000,
    0.000000, 0.000000, 1.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 1.000000,
};

// const Matrix T12 = MatrixMultiply(
    // MatrixTranslate(0.0, 0.12, 0.0),
    // MatrixRotateZYX({PI, 0.0, -PI/2})
// );
const Matrix T12 = MatrixMultiply(
    MatrixTranslate(0.0, 0.0, -0.12),
    MatrixRotateXYZ({0.0, PI, 0.0})
);

// const Matrix T23 = MatrixMultiply(
    // MatrixTranslate(-0.457, 0.09, 0.0),
    // MatrixRotateXYZ({PI, 0.0, 0.0})
// );
const Matrix T23 = MatrixMultiply(
    MatrixTranslate(0.0, 0.245, -0.09),
    MatrixRotateXYZ({0.0, PI, 0.0})
);

// const Matrix T34 = MatrixMultiply(
    // MatrixTranslate(-0.1, 0.0, 0.0),
    // MatrixRotateXYZ({0.0, PI, PI/2})
// );
const Matrix T34 = MatrixMultiply(
    MatrixTranslate(0.0, 0.212, -0.1),
    MatrixRotateXYZ({0.0, PI, 0.0})
);

// const Matrix T45 = MatrixMultiply(
    // MatrixTranslate(-0.085, 0.0, 0.0),
    // MatrixRotateXYZ({0.0, PI, PI/2})
// );
const Matrix T45 = MatrixMultiply(
    MatrixTranslate(0.0, 0.0, -0.085),
    MatrixRotateXYZ({-PI/2, 0.0, PI})
);

// const Matrix T56 = MatrixMultiply(
    // MatrixTranslate(0.0, 0.0, -0.09),
    // MatrixRotateXYZ({-PI/2, 0.0, 0.0})
// );
const Matrix T56 = MatrixMultiply(
    MatrixTranslate(0.0, 0.0, -0.072),
    MatrixRotateXYZ({-PI/2, 0.0, 0.0})
);

#endif

