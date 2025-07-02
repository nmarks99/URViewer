#pragma once

struct UIFlags {
    bool show_axes = false;
};

class MenuPanel {
  public:
    MenuPanel(int screen_width, int screen_height);
    void update();
    void draw();

    UIFlags flags;

  private:
    float x_;
    float y_;
    float screen_width_;
    float screen_height_;
    bool visible_ = true;
};
