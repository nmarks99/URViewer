#pragma once
#include "ur.hpp"
#include <array>

struct UIFlags {
    bool show_axes = false;
    int show_axes_mask = false;
    std::array<bool, UR_NUM_AXES> axes_mask_arr;
};

class MenuPanel {
  public:
    MenuPanel();
    void update();
    void draw();

    UIFlags flags;

  private:
    float screen_width_;
    float screen_height_;
    float x_;
    float y_;
    bool visible_ = true;
};
