// UR3e
// This file uses raymath.h to create 4x4 homogenous transformation
// matrices which define the position and orientation of each of the robot
// links in the kinematic chain
//
// Note: Do not take the below transforms as truth. They were chosen
// experimentally based on what is *visually* correct

#ifndef _UR3E_HPP
#define _UR3E_HPP

#include "raylib.h"
#include "raymath.h"

const Matrix TSBASE = MatrixRotateX(-PI/2);

const Matrix TB1 = MatrixMultiply(
    MatrixTranslate(0.0, 0.15, 0.0),
    MatrixRotateXYZ({PI/2, 0.0, 0.0})
);

const Matrix T12 = MatrixMultiply(
    MatrixTranslate(0.0, 0.0, -0.12),
    MatrixRotateXYZ({0.0, PI, 0.0})
);

const Matrix T23 = MatrixMultiply(
    MatrixTranslate(0.0, 0.245, -0.09),
    MatrixRotateXYZ({0.0, PI, 0.0})
);

const Matrix T34 = MatrixMultiply(
    MatrixTranslate(0.0, 0.212, -0.1),
    MatrixRotateXYZ({0.0, PI, 0.0})
);

const Matrix T45 = MatrixMultiply(
    MatrixTranslate(0.0, 0.0, -0.085),
    MatrixRotateXYZ({-PI/2, 0.0, PI})
);

const Matrix T56 = MatrixMultiply(
    MatrixTranslate(0.0, 0.0, -0.072),
    MatrixRotateXYZ({-PI/2, 0.0, 0.0})
);

#endif

