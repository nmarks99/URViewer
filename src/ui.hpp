#pragma once
#include <vector>
#include "i_comm.hpp"

constexpr int TEXT_INPUT_SIZE = 128;

struct UIState {
    bool show_axes = false;
    int axes_mask = 0;
    int wires_mask = 0;

    char conn_string[128] = "164.54.104.148";
    bool conn_text_edit_mode = false;
    bool connect_called = false;
    bool disconnect_called = false;

    bool ask_to_quit = false;
    bool exit_window = false;
};

class Ui {
  public:
    Ui();
    void update(const RobotState &robot_state);
    void draw();

    UIState state;

  private:
    float screen_width_;
    float screen_height_;
    float x_;
    float y_;
    std::vector<float> qvec_{6, 0.0};
    bool connected_ = false;
};
