#include "ui.hpp"
#include "raygui.h"
#include "raylib.h"
#include "ur.hpp"

constexpr int MENU_WIDTH = 430;
constexpr int MENU_HEIGHT = 490;
constexpr int DEFAULT_FONT_SIZE = 24;

Ui::Ui(const UIState &ui_state)
    : screen_width_(GetScreenWidth()), screen_height_(GetScreenHeight()), x_(screen_width_ - MENU_WIDTH),
      y_(0.0), state(ui_state) {
    // Load fonts
    std::filesystem::path resource_dir = URVIEWER_RESOURCE_DIR;
    std::vector<int> codepoints_vec;
    for (int i = 32; i <= 126; ++i) {
        codepoints_vec.push_back(i);
    }
    codepoints_vec.push_back(0xE33E); // degree
    codepoints_vec.push_back(0xf467); // X
    codepoints_vec.push_back(0xf00c); // checkmark
    font_ = LoadFontEx(
        (std::filesystem::path(URVIEWER_RESOURCE_DIR) / "fonts/JetBrainsMonoNerdFont-Regular.ttf").c_str(),
        24, codepoints_vec.data(), codepoints_vec.size());
    GuiSetFont(font_);
    GuiSetStyle(DEFAULT, TEXT_SIZE, DEFAULT_FONT_SIZE);
}

Ui::~Ui() {
    if (IsFontValid(font_)) {
        UnloadFont(font_);
    }
}

void Ui::update(const RobotState &robot_state) {
    connected_ = robot_state.connected;
    qvec_ = robot_state.joint_angles;
    for (float &v : qvec_) {
        v *= RAD2DEG;
    }
    screen_width_ = static_cast<float>(GetScreenWidth());
    screen_height_ = static_cast<float>(GetScreenHeight());
    x_ = static_cast<float>(screen_width_ - MENU_WIDTH);
    y_ = 0.0;

    // ask if user really wants to exit
    if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) {
        state.ask_to_quit = !state.ask_to_quit;
    }
}

void Ui::draw() {

    // Box to contain all menu elements
    if (state.show_menu) {
        int res = GuiWindowBox(Rectangle{x_, y_, MENU_WIDTH, MENU_HEIGHT}, "Settings");
        if (res) {
            state.show_menu = false;
        }
    } else {
        if (GuiButton((Rectangle){screen_width_ - 135, 10, 125, 30}, GuiIconText(ICON_GEAR, "Settings"))) {
            state.show_menu = true;
        }
        return;
    }

    // Connection string (IP addr/PV prefix) input box
    constexpr float CONN_REC_WIDTH = 250.0;
    // constexpr float WIDGET_X_OFFSET = float(MENU_WIDTH/2.0 - CONN_REC_WIDTH/2.0);
    constexpr float WIDGET_X_OFFSET = 140;
    if (GuiTextBox(Rectangle{.x = x_ + WIDGET_X_OFFSET, .y = y_ + 100, .width = CONN_REC_WIDTH, .height = 25},
                   state.connection_string.data(), TEXT_INPUT_SIZE, state.conn_text_active)) {
        state.conn_text_active = !state.conn_text_active;
    }

    // Connect button
    if (GuiButton(
            Rectangle{
                .x = x_ + WIDGET_X_OFFSET, .y = y_ + 130, .width = CONN_REC_WIDTH / 2 - 5, .height = 25},
            "Connect")) {
        if (!state.backend_menu_active) {
            state.connect_called = true;
        }
    }

    // Disconnect button
    if (GuiButton(Rectangle{.x = x_ + WIDGET_X_OFFSET + CONN_REC_WIDTH / 2 + 5,
                            .y = y_ + 130,
                            .width = CONN_REC_WIDTH / 2 - 5,
                            .height = 25},
                  "Disconnect")) {
        state.disconnect_called = true;
    }

    std::string connection_status_msg;
    if (connected_) {
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(GREEN));
        connection_status_msg = "";
    } else {
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(RED));
        connection_status_msg = "";
    }
    GuiLabel(
        Rectangle{.x = x_ + WIDGET_X_OFFSET + CONN_REC_WIDTH + 3, .y = y_ + 100, .width = 24, .height = 24},
        connection_status_msg.data());
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

    GuiLabel(Rectangle{x_ + WIDGET_X_OFFSET - 130, y_ + 40, 150, 25}, "     Robot: ");
    GuiLabel(Rectangle{x_ + WIDGET_X_OFFSET - 130, y_ + 70, 150, 25}, "   Backend: ");
    if (state.backend_menu_selected == 0) {
        GuiLabel(Rectangle{x_ + WIDGET_X_OFFSET - 130, y_ + 100, 150, 25}, "IP address: ");
    } else {
        GuiLabel(Rectangle{x_ + WIDGET_X_OFFSET - 130, y_ + 100, 150, 25}, "IOC prefix: ");
    }

    // Dropdown for communciation backend selection
    if (GuiDropdownBox(Rectangle{x_ + WIDGET_X_OFFSET, y_ + 70, 125, 25}, "TCP/IP;EPICS",
                       &state.backend_menu_selected, state.backend_menu_active)) {
        if (!state.model_menu_active) {
            state.backend_menu_active = !state.backend_menu_active;
        }
    }

    // Dropdown for robot model selection
    if (GuiDropdownBox(Rectangle{x_ + WIDGET_X_OFFSET, y_ + 40, 125, 25}, "UR3e;UR5e",
                       &state.model_menu_selected, state.model_menu_active)) {
        state.model_menu_active = !state.model_menu_active;
    }

    // ------------------------------------------------------------------
    // Robot data/settings
    // ------------------------------------------------------------------

    static constexpr float s2_y_start = 175;
    // axes check box labels
    GuiLabel(
        Rectangle{
            .x = x_ + 235,
            .y = y_ + s2_y_start,
            .width = 100,
            .height = 30,
        },
        "Axes");

    // wires check box labels
    GuiLabel(
        Rectangle{
            .x = x_ + 295,
            .y = y_ + s2_y_start,
            .width = 100,
            .height = 30,
        },
        "Wires");

    // Draw each row of the menu panel
    const float row_offset = 30;
    const float top_offset = s2_y_start + 30;
    for (int row = 0; row < UR_NUM_MODELS; row++) {
        // Model label
        GuiLabel(
            Rectangle{
                .x = x_ + 20,
                .y = y_ + top_offset + float(row_offset * row),
                .width = 100,
                .height = 30,
            },
            UR_MODEL_LABELS.at(row).data());

        // Joint angle (first model is base)
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(DARKBLUE));
        if (row > 0 and row <= UR_NUM_AXES) {
            std::ostringstream oss;
            oss.precision(2);
            oss << std::fixed;
            const float val = qvec_.at(row - 1);
            if (val > 0) {
                oss << " ";
            }
            oss << val << "";
            GuiLabel(
                Rectangle{
                    .x = x_ + 140,
                    .y = y_ + top_offset + float(row_offset * row),
                    .width = 100,
                    .height = 30,
                },
                oss.str().c_str());
        }
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

        // check box to toggle coordinate axis
        bool checked_axis = (state.axes_mask >> row) & 1;
        if (GuiCheckBox(Rectangle{.x = x_ + 250,
                                  .y = y_ + (top_offset + row_offset - 22) + float(row_offset * row),
                                  .width = 15,
                                  .height = 15},
                        "", &checked_axis)) {
            checked_axis ? state.axes_mask |= (1 << row) : state.axes_mask &= ~(1 << row);
        }

        // check box to toggle wireframe drawing mode
        bool checked_wire = (state.wires_mask >> row) & 1;
        if (GuiCheckBox(Rectangle{.x = x_ + 315,
                                  .y = y_ + (top_offset + row_offset - 22) + float(row_offset * row),
                                  .width = 15,
                                  .height = 15},
                        "", &checked_wire)) {
            checked_wire ? state.wires_mask |= (1 << row) : state.wires_mask &= ~(1 << row);
        }
    }

    // Ask to quit box
    if (state.ask_to_quit) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
        int result = GuiMessageBox(
            (Rectangle){(float)GetScreenWidth() / 2 - 225, (float)GetScreenHeight() / 2 - 100, 450, 200},
            GuiIconText(ICON_EXIT, "Close Window"), "Do you really want to exit?", "Yes;No");
        if ((result == 0) || (result == 2)) {
            state.ask_to_quit = false;
        } else if (result == 1) {
            state.exit_window = true;
        }
    }
}
