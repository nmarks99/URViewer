#include "rl_utils.hpp"
#include "raymath.h"

RLModel::RLModel(const char *model_path) : model(LoadModel(model_path)) {}

RLModel::RLModel(std::filesystem::path model_path) : model(LoadModel(model_path.string().c_str())) {}

RLModel::RLModel(const char *model_path, const std::string &name)
    : model(LoadModel(model_path)), name(name) {}

RLModel::RLModel(std::filesystem::path model_path, const std::string &name)
    : model(LoadModel(model_path.string().c_str())), name(name) {}

RLModel::~RLModel() {
    if (model.meshCount > 0) {
        TraceLog(LOG_INFO, "Unloading model\n");
        UnloadModel(model);
    }
}

void RLModel::draw() { DrawModel(model, Vector3Zeros, 1.0, WHITE); }

void RLModel::draw_wires() { DrawModelWires(model, Vector3Zeros, 1.0, WHITE); }

void RLModel::draw_axes() { draw_axes_3d(0.005, model.transform); }


RLWindow::RLWindow(int width, int height, const char *title) { InitWindow(width, height, title); };

RLWindow::~RLWindow() { CloseWindow(); }


void draw_axes_3d(float thickness, Matrix transform) {
    constexpr int sides = 20;
    const Vector3 origin = Vector3Transform(Vector3Zeros, transform);
    const Vector3 x_end = Vector3Transform({0.1, 0.0, 0.0}, transform);
    const Vector3 y_end = Vector3Transform({0.0, 0.1, 0.0}, transform);
    const Vector3 z_end = Vector3Transform({0.0, 0.0, 0.1}, transform);
    DrawCylinderEx(origin, x_end, thickness, thickness, sides, RED);
    DrawCylinderEx(origin, y_end, thickness, thickness, sides, GREEN);
    DrawCylinderEx(origin, z_end, thickness, thickness, sides, BLUE);
}
