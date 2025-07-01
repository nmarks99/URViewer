#pragma once
#include "raylib.h"
#include <filesystem>

struct RLModel {
  public:
    Model model;

    RLModel(const char *model_path);
    RLModel(std::filesystem::path model_path);
    ~RLModel();

    // delete copy constructor and copy assignment operator
    RLModel(const RLModel &) = delete;
    RLModel &operator=(const RLModel &) = delete;

    void draw();
    void draw_wires();
    void draw_axes();
};

struct RLWindow {
    RLWindow(int width, int height, const char *title);

    ~RLWindow();

    // delete copy constructor and copy assignment operator
    RLWindow(const RLWindow &) = delete;
    RLWindow &operator=(const RLWindow &) = delete;
};

void draw_axes_3d(float thickness, Matrix transform);
