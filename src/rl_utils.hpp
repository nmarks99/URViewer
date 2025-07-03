#pragma once
#include "raylib.h"
#include <filesystem>

struct RLModel {
    Model model;

    RLModel(const char *model_path);
    RLModel(std::filesystem::path model_path);
    RLModel(const char *model_path, const std::string &name);
    RLModel(std::filesystem::path model_path, const std::string &name);
    ~RLModel();

    // delete copy constructor and copy assignment operator
    RLModel(const RLModel &) = delete;
    RLModel &operator=(const RLModel &) = delete;

    void draw();
    void draw(Color color);
    void draw_wires();
    void draw_axes();
  
    std::string name = "";
};

struct RLWindow {
    RLWindow(int width, int height, const char *title);

    ~RLWindow();

    // delete copy constructor and copy assignment operator
    RLWindow(const RLWindow &) = delete;
    RLWindow &operator=(const RLWindow &) = delete;
};

void draw_axes_3d(float thickness, Matrix transform);
