#pragma once
#include <filesystem>
#include <vector>

#include "utils.hpp"
#include "raymath.h"

enum class URVersion {
    UR3e,
    UR5e, // TODO: support this
};

class UR {
  public:
    UR(URVersion version);
    void draw();
    void draw_axes();
    void update(const std::vector<double> &joint_angles);

  private:
    std::filesystem::path model_dir_;
    RLModel base;
    RLModel link1;
    RLModel link2;
    RLModel link3;
    RLModel link4;
    RLModel link5;
    RLModel link6;
};


namespace UR3e {
    const Matrix TSBASE = MatrixRotateX(-PI/2);

    const Matrix TB1 = MatrixMultiply(
        MatrixTranslate(0.0, 0.0, 0.15),
        MatrixRotateXYZ({0.0, 0.0, 0.0})
    );

    const Matrix T12 = MatrixMultiply(
        MatrixTranslate(0.0, 0.0, 0.12),
        MatrixRotateXYZ({-PI/2, 0.0, -PI/2})
    );

    const Matrix T23 = MatrixMultiply(
        MatrixTranslate(0.0, 0.245, 0.09),
        MatrixRotateXYZ({0.0, PI, 0.0})
    );

    const Matrix T34 = MatrixMultiply(
        MatrixTranslate(0.0, 0.212, 0.1),
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
}
