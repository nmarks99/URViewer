#include "raylib.h"
#include "raygui.h"
#include "ui.hpp"
#include "ur.hpp"

constexpr int MENU_WIDTH = 300;

MenuPanel::MenuPanel() 
    : screen_width_(GetScreenWidth()),
    screen_height_(GetScreenHeight()),
    x_(screen_width_ - MENU_WIDTH),
    y_(0.0)
{
}

void MenuPanel::update() {
    screen_width_ = static_cast<float>(GetScreenWidth());
    screen_height_ = static_cast<float>(GetScreenHeight());
    x_ = static_cast<float>(screen_width_-MENU_WIDTH);
    y_ = 0.0;
}

void MenuPanel::update(const std::vector<float> &joint_angles) {
    qvec_ = joint_angles;
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
        .x = x_+95,
        .y = y_+20,
        .width = 100,
        .height = 30,
    }, "Axes");

    // wires check box labels
    GuiLabel(Rectangle{
        .x = x_+150,
        .y = y_+20,
        .width = 100,
        .height = 30,
    }, "Wires");

    // model names labels column
    for (int i = 0; i < UR_NUM_MODELS; i++) {
        GuiLabel(Rectangle{
            .x = x_+10,
            .y = y_+40+float(20.0*i),
            .width = 100,
            .height = 30,
        }, UR_MODEL_LABELS.at(i).data());
    }

    // check boxes for toggling model coordinate axes
    for (int i = 0; i < UR_NUM_MODELS; i++) {
        bool checked = (flags.axes_mask >> i) & 1;
        if (GuiCheckBox(
            Rectangle {
                .x = x_+100,
                .y = y_+46+float(20.0*i),
                .width = 15,
                .height = 15
            }, "", &checked)) {
            checked ? flags.axes_mask |= (1 << i) : flags.axes_mask &= ~(1 << i);
        }
    }

    // check boxes for toggling drawing in wireframe mode
    for (int i = 0; i < UR_NUM_MODELS; i++) {
        bool checked = (flags.wires_mask >> i) & 1;
        if (GuiCheckBox(
            Rectangle {
                .x = x_+150,
                .y = y_+46+float(20.0*i),
                .width = 15,
                .height = 15
            }, "", &checked)) {
            checked ? flags.wires_mask |= (1 << i) : flags.wires_mask &= ~(1 << i);
        }
    }
}
