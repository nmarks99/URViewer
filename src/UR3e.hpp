#ifndef _UR3E_HPP
#define _UR3E_HPP

#include "raylib.h"
#include "raymath.h"

// constexpr Matrix TSBASE {
    // 1.000000, 0.000000, 0.000000, 0.000000,
    // 0.000000, 1.000000, 0.000000, 0.000000,
    // 0.000000, 0.000000, 1.000000, 0.000000,
    // 0.000000, 0.000000, 0.000000, 1.000000,
// };

constexpr Matrix TS1 {
    1.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 1.000000, 0.000000, 0.157000,
    0.000000, 0.000000, 1.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 1.000000,
};

const Matrix T12 = MatrixMultiply(
    MatrixTranslate(0.0, 0.12, 0.0),
    MatrixRotateZYX({PI, 0.0, -PI/2})
);

const Matrix T23 = MatrixMultiply(
    MatrixTranslate(-0.457, 0.09, 0.0),
    MatrixRotateXYZ({PI, 0.0, 0.0})
);

const Matrix T34 = MatrixMultiply(
    MatrixTranslate(-0.1, 0.0, 0.0),
    MatrixRotateXYZ({0.0, PI, PI/2})
);

const Matrix T45 = MatrixMultiply(
    MatrixTranslate(-0.085, 0.0, 0.0),
    MatrixRotateXYZ({0.0, PI, PI/2})
);

const Matrix T56 = MatrixMultiply(
    MatrixTranslate(0.0, 0.0, -0.09),
    MatrixRotateXYZ({-PI/2, 0.0, 0.0})
);

// constexpr Matrix TS2 {
    // 0.000796, -0.999998, -0.001593, -0.120000,
    // -1.000000, -0.000796, -0.000001, 0.156904,
    // 0.000000, 0.001593, -0.999999, 0.000191,
    // 0.000000, 0.000000, 0.000000, 1.000000,
// };
//
// constexpr Matrix TS3 {
    // 0.000796, 0.999994, 0.003186, -0.020364,
    // -1.000000, 0.000796, 0.000002, 0.613984,
    // 0.000000, -0.003186, 0.999995, -0.000128,
    // 0.000000, 0.000000, 0.000000, 1.000000
// };

// const Matrix T12 = MatrixMultiply(MatrixInvert(TS1), TS2);
// const Matrix T23 = MatrixMultiply(MatrixInvert(TS2), TS3);

constexpr Matrix Rx(float th) {
    using namespace std;
    return Matrix {
        1.0,      0.0,     0.0, 0.0,
        0.0,  cos(th), sin(th), 0.0,
        0.0, -sin(th), cos(th), 0.0,
        0.0,      0.0,     0.0, 1.0,
    };
}

constexpr Matrix Ry(float th) {
    using namespace std;
     return Matrix {
        cos(th), 0.0, -sin(th), 0.0,
        sin(th), 1.0,  cos(th), 0.0,
            0.0, 0.0,      0.0, 0.0,
            0.0, 0.0,      0.0, 1.0,
    };
}

constexpr Matrix Rz(float th) {
    using namespace std;
     return Matrix {
        cos(th), -sin(th), 0.0, 0.0,
        sin(th),  cos(th), 0.0, 0.0,
            0.0,      0.0, 1.0, 0.0,
            0.0,      0.0, 0.0, 1.0,
    };
}



#endif

