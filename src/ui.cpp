#include "raylib.h"
#include "raygui.h"
#include "ui.hpp"
#include "ur.hpp"

constexpr int MENU_WIDTH = 400;

MenuPanel::MenuPanel() 
    : screen_width_(GetScreenWidth()),
    screen_height_(GetScreenHeight()),
    x_(screen_width_ - MENU_WIDTH),
    y_(0.0)
{}

void MenuPanel::update() {
    screen_width_ = static_cast<float>(GetScreenWidth());
    screen_height_ = static_cast<float>(GetScreenHeight());
    x_ = static_cast<float>(screen_width_-MENU_WIDTH);
    y_ = 0.0;
}

void MenuPanel::update(const std::vector<float> &joint_angles) {
    qvec_ = joint_angles;
    for (float &v : qvec_) {
        v *= RAD2DEG;
    }
    screen_width_ = static_cast<float>(GetScreenWidth());
    screen_height_ = static_cast<float>(GetScreenHeight());
    x_ = static_cast<float>(screen_width_-MENU_WIDTH);
    y_ = 0.0;
}

void MenuPanel::draw() {

    // Panel to contain all menu elements
    GuiWindowBox(
        Rectangle {
            .x = screen_width_-MENU_WIDTH,
            .y = 0,
            .width = MENU_WIDTH,
            .height = screen_height_,
        },
        "Menu"
    );

    // axes check box labels
    GuiLabel(Rectangle{
        .x = x_+225,
        .y = y_+30,
        .width = 100,
        .height = 30,
    }, "Axes");

    // wires check box labels
    GuiLabel(Rectangle{
        .x = x_+285,
        .y = y_+30,
        .width = 100,
        .height = 30,
    }, "Wires");

    // Draw each row of the menu panel
    static const float row_offset = 30;
    static const float top_offset = 60;
    for (int row = 0; row < UR_NUM_MODELS; row++) {
        // Model label
        GuiLabel(Rectangle{
            .x = x_+10,
            .y = y_+top_offset+float(row_offset*row),
            .width = 100,
            .height = 30,
        }, UR_MODEL_LABELS.at(row).data());


        // Joint angle (first model is base)
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(DARKBLUE));
        if (row > 0) {
            std::ostringstream oss;
            oss.precision(2);
            oss << std::fixed << qvec_.at(row - 1);
            GuiLabel(Rectangle{
                .x = x_+140,
                .y = y_+top_offset+float(row_offset*row),
                .width = 100,
                .height = 30,
            }, oss.str().c_str());
        }
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

        // check box to toggle coordinate axis
        bool checked_axis = (flags.axes_mask >> row) & 1;
        if (GuiCheckBox(
            Rectangle {
                .x = x_+240,
                .y = y_+(top_offset+row_offset-22)+float(row_offset*row),
                .width = 15,
                .height = 15
            }, "", &checked_axis)) {
            checked_axis ? flags.axes_mask |= (1 << row) : flags.axes_mask &= ~(1 << row);
        }

        // check box to toggle wireframe drawing mode
        bool checked_wire = (flags.wires_mask >> row) & 1;
        if (GuiCheckBox(
            Rectangle {
                .x = x_+305,
                .y = y_+(top_offset+row_offset-22)+float(row_offset*row),
                .width = 15,
                .height = 15
            }, "", &checked_wire)) {
            checked_wire ? flags.wires_mask |= (1 << row) : flags.wires_mask &= ~(1 << row);
        }

    }
}
