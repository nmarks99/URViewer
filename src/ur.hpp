#pragma once
#include <filesystem>
#include <vector>

#include "rl_utils.hpp"
#include "raymath.h"

constexpr int UR_NUMBER_OF_AXES = 6;

enum class URVersion {
    UR3e,
    UR5e, // TODO: support this
};

class UR {
  public:
    UR(URVersion version);
    void draw();
    void draw_axes();
    void draw_axes(int mask);
    void update(const std::vector<float> &joint_angles);

  private:
    std::filesystem::path model_dir_;
    RLModel base;
    RLModel shoulder;
    RLModel upperarm;
    RLModel forearm;
    RLModel wrist1;
    RLModel wrist2;
    RLModel wrist3;

    // used to "index" the joints
    RLModel& at(int index);
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
        MatrixTranslate(0.0, 0.245, -0.09),
        MatrixRotateXYZ({0.0, 0.0, 0.0})
    );

    const Matrix T34 = MatrixMultiply(
        // MatrixTranslate(-0.212, 0.0, 0.1),
        // MatrixRotateXYZ({0.0, PI, -PI/2})
        MatrixTranslate(-0.212, 0.0, 0.1),
        MatrixRotateXYZ({0.0, 0.0, -PI/2})
    );

    const Matrix T45 = MatrixMultiply(
        MatrixTranslate(0.0, 0.0, 0.085),
        MatrixRotateXYZ({PI/2, 0.0, PI})
    );

    const Matrix T56 = MatrixMultiply(
        MatrixTranslate(0.0, 0.0, 0.072),
        MatrixRotateXYZ({PI/2, 0.0, 0.0})
    );
}
