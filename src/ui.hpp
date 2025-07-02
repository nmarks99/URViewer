#pragma once
#include <vector>

struct UIFlags {
    bool show_axes = false;
    int axes_mask = 0;
    int wires_mask = 0;
};

class MenuPanel {
  public:
    MenuPanel();
    void update();
    void update(const std::vector<float> &joint_angles);
    void draw();

    UIFlags flags;

  private:
    float screen_width_;
    float screen_height_;
    float x_;
    float y_;
    std::vector<float> qvec_{6, 0.0};
};
