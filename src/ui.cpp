#include "raylib.h"
#include "raygui.h"
#include "ui.hpp"
#include "ur.hpp"

constexpr int MENU_WIDTH = 400;
constexpr int MENU_HEIGHT = 450;

Ui::Ui() 
    : screen_width_(GetScreenWidth()),
    screen_height_(GetScreenHeight()),
    x_(screen_width_ - MENU_WIDTH),
    y_(0.0)
{}

void Ui::update(const RobotState &robot_state) {
    connected_ = robot_state.connected;
    qvec_ = robot_state.joint_angles;
    for (float &v : qvec_) {
        v *= RAD2DEG;
    }
    screen_width_ = static_cast<float>(GetScreenWidth());
    screen_height_ = static_cast<float>(GetScreenHeight());
    x_ = static_cast<float>(screen_width_-MENU_WIDTH);
    y_ = 0.0;
}

void Ui::draw() {

    // Box to contain all menu elements
    GuiPanel(Rectangle{x_, y_, MENU_WIDTH, MENU_HEIGHT}, "Menu");

    // ------------------------------------------------------------------
    // Connection 
    // ------------------------------------------------------------------

    // Connection string (IP addr/PV prefix) input box
    static constexpr float CONN_REC_WIDTH = 250.0;
    if (GuiTextBox(Rectangle{
        .x = x_+float(MENU_WIDTH/2.0 - CONN_REC_WIDTH/2.0),
        .y = y_+40,
        .width = CONN_REC_WIDTH,
        .height = 25
    }, state.conn_string, TEXT_INPUT_SIZE, state.conn_text_edit_mode)) {
        state.conn_text_edit_mode = !state.conn_text_edit_mode;
    }

    if (GuiButton(Rectangle{
        .x = x_+float(MENU_WIDTH/2.0 - CONN_REC_WIDTH/2.0),
        .y = y_+70,
        .width = CONN_REC_WIDTH/2-5,
        .height = 25
    }, "Connect")) {
        state.connect_called = true;
    }

    if (GuiButton(Rectangle{
        .x = x_+float(MENU_WIDTH/2.0+5),
        .y = y_+70,
        .width = CONN_REC_WIDTH/2-5,
        .height = 25
    }, "Disconnect")) {
        state.disconnect_called = true;
    }

    std::string connection_status_msg;
    if (connected_) {
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(GREEN));
        connection_status_msg = "  Connected  ";
    } else {
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(RED));
        connection_status_msg = " Disconnected";
    }
    GuiLabel(Rectangle {
        .x = x_+float(MENU_WIDTH/2.0 - CONN_REC_WIDTH/2.0) + 48,
        .y = y_+100,
        .width = screen_width_,
        .height = 30
    }, connection_status_msg.data());
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));


    // ------------------------------------------------------------------
    // Robot data/settings 
    // ------------------------------------------------------------------

    static constexpr float s2_y_start = 150;
    // axes check box labels
    GuiLabel(Rectangle{
        .x = x_+235,
        .y = y_+s2_y_start,
        .width = 100,
        .height = 30,
    }, "Axes");

    // wires check box labels
    GuiLabel(Rectangle{
        .x = x_+295,
        .y = y_+s2_y_start,
        .width = 100,
        .height = 30,
    }, "Wires");

    // Draw each row of the menu panel
    const float row_offset = 30;
    const float top_offset = s2_y_start + 30;
    for (int row = 0; row < UR_NUM_MODELS; row++) {
        // Model label
        GuiLabel(Rectangle{
            .x = x_+20,
            .y = y_+top_offset+float(row_offset*row),
            .width = 100,
            .height = 30,
        }, UR_MODEL_LABELS.at(row).data());


        // Joint angle (first model is base)
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(DARKBLUE));
        if (row > 0 and row <= 6) {
            std::ostringstream oss;
            oss.precision(2);
            oss << std::fixed << qvec_.at(row - 1);
            GuiLabel(Rectangle{
                .x = x_+150,
                .y = y_+top_offset+float(row_offset*row),
                .width = 100,
                .height = 30,
            }, oss.str().c_str());
        }
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

        // check box to toggle coordinate axis
        bool checked_axis = (state.axes_mask >> row) & 1;
        if (GuiCheckBox(
            Rectangle {
                .x = x_+250,
                .y = y_+(top_offset+row_offset-22)+float(row_offset*row),
                .width = 15,
                .height = 15
            }, "", &checked_axis)) {
            checked_axis ? state.axes_mask |= (1 << row) : state.axes_mask &= ~(1 << row);
        }

        // check box to toggle wireframe drawing mode
        bool checked_wire = (state.wires_mask >> row) & 1;
        if (GuiCheckBox(
            Rectangle {
                .x = x_+315,
                .y = y_+(top_offset+row_offset-22)+float(row_offset*row),
                .width = 15,
                .height = 15
            }, "", &checked_wire)) {
            checked_wire ? state.wires_mask |= (1 << row) : state.wires_mask &= ~(1 << row);
        }
    }
   

    // ------------------------------------------------------------------
    // Ask to quit box
    // ------------------------------------------------------------------

    if (state.ask_to_quit) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
        int result = GuiMessageBox(
            (Rectangle){ (float)GetScreenWidth()/2 - 225, (float)GetScreenHeight()/2 - 100, 450, 200 },
            GuiIconText(ICON_EXIT, "Close Window"),
            "Do you really want to exit?", "Yes;No");
        if ((result == 0) || (result == 2)) {
            state.ask_to_quit = false;
        } else if (result == 1) {
            state.exit_window = true;
        }
    }
}
