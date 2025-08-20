#pragma once
#include <vector>
#include "raylib.h"
#include "comm_base.hpp"

constexpr int TEXT_INPUT_SIZE = 128;

struct UIState {
    bool show_axes = false;
    int axes_mask = 0;
    int wires_mask = 0;

    std::string connection_string;
    bool conn_text_active = false;
    bool connect_called = false;
    bool disconnect_called = false;

    bool ask_to_quit = false;
    bool exit_window = false;

    bool show_menu = true;

    UIState() {
        connection_string.resize(TEXT_INPUT_SIZE, '\0');
    }
};

class Ui {
  public:
    Ui(const UIState &state);
    ~Ui();
    void update(const RobotState &robot_state);
    void draw();
    UIState state;

  private:
    Font font_;
    float screen_width_;
    float screen_height_;
    float x_;
    float y_;
    std::vector<float> qvec_{6, 0.0};
    bool connected_ = false;
};
