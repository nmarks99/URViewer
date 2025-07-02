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

void MenuPanel::draw() {

    // Panel to contain all menu elements
    GuiPanel(
        Rectangle {
            .x = screen_width_-MENU_WIDTH,
            .y = 0,
            .width = MENU_WIDTH,
            .height = screen_height_,
        },
        "Menu"
    );

    // check boxes for showing/hiding model coordinate axes
    for (int i = 0; i < UR_NUM_AXES; i++) {
        bool checked = (flags.axes_mask >> i) & 1;
        if (GuiCheckBox(
            Rectangle {
                .x = x_+10,
                .y = 40+float(20.0*i),
                .width = 15,
                .height = 15
            }, UR_MODEL_LABELS.at(i+1).data(), &checked)) {
            checked ? flags.axes_mask |= (1 << i) : flags.axes_mask &= ~(1 << i);
        }
    }
}
