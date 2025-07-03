#pragma once
#include <filesystem>
#include <vector>

#include "rl_utils.hpp"
#include "raymath.h"

constexpr int UR_NUM_AXES = 6;
constexpr int UR_NUM_MODELS = UR_NUM_AXES + 1; // 6 axes/links plus base

constexpr std::array<std::string_view, UR_NUM_MODELS> UR_MODEL_LABELS = {
    "Base",
    "Shoulder",
    "Upperarm",
    "Forearm",
    "Wrist1",
    "Wrist2",
    "Wrist3",
};

enum class URVersion {
    UR3e,
    UR5e, // TODO: support this
};

class UR {
  public:
    UR(URVersion version);
    void draw();
    void draw(int mask, bool opaque = false);
    void draw_axes();
    void draw_axes(int mask);
    void update(const std::vector<float> &joint_angles);
    
    // used to "index" the joints
    RLModel& at(int index);

  private:
    std::filesystem::path model_dir_;
    RLModel base_;
    RLModel shoulder_;
    RLModel upperarm_;
    RLModel forearm_;
    RLModel wrist1_;
    RLModel wrist2_;
    RLModel wrist3_;
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
