#include "raylib.h"
#include "raygui.h"
#include "ui.hpp"

constexpr int MENU_WIDTH = 300;

MenuPanel::MenuPanel(int screen_width, int screen_height) 
    : screen_width_(screen_width),
    screen_height_(screen_height),
    x_(screen_width - MENU_WIDTH),
    y_(0)
{}

void MenuPanel::update() {
    screen_width_ = static_cast<float>(GetScreenWidth());
    screen_height_ = static_cast<float>(GetScreenHeight());
    x_ = static_cast<float>(screen_width_-MENU_WIDTH);
    y_ = 0.0;
}

void MenuPanel::draw() {
    GuiPanel(
        Rectangle {
            .x = screen_width_-MENU_WIDTH,
            .y = 0,
            .width = MENU_WIDTH,
            .height = screen_height_,
        },
        "Menu"
    );

    // check box to show/hide axes
    GuiCheckBox(
        Rectangle {
            .x = x_+10,
            .y = 40,
            .width = 15,
            .height = 15
        },
        "Show axes", &flags.show_axes
    );
}
