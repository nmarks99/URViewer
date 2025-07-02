#include <string>

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "rl_utils.hpp"
#include "ur.hpp"
#include "ui.hpp"
#include "comm.hpp"

int main(void) {

    // initialize the window
    SetTraceLogLevel(LOG_FATAL);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    RLWindow window(1000, 800, "UR Robot Viewer");
    
    // GuiSetStyle(DEFAULT, TEXT_SIZE, 18);

    // Define the camera to look into our 3d world
    Camera camera = {0};
    camera.position = Vector3{-1.0f, 1.0f, -1.25f}; // Camera position
    camera.target = Vector3{0.0f, 0.25f, 0.0f};  // Camera looking at point
    camera.up = Vector3{0.0f, 1.0f, 0.0f};       // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                         // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;      // Camera mode type

    // Load models and apply initial transforms
    UR robot(URVersion::UR3e);

    // Connection to the robot
    URRtdeSource ur_comm;
    ur_comm.connect("164.54.104.148");

    // vector to store joint angles
    std::vector<float> qvec(6);

    // UI
    MenuPanel menu(GetScreenWidth(), GetScreenHeight());

    // bool show_axes = false;
    bool ask_to_quit_box = false;
    bool exit_window = false;
    std::string ip_addr_str(100, '\0');

    while (!exit_window) {

        // UPDATE ///////////////////////////////////////////////
        // ask if user really wants to exit
        if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) {
            ask_to_quit_box = !ask_to_quit_box;
        }

        // for adjusting the camera
        if (IsKeyDown(KEY_LEFT_CONTROL) or IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        }

        // get joint angles from robot
        if (ur_comm.connected()) {
            qvec = ur_comm.get_joint_angles();
            robot.update(qvec);
        }

        menu.update();
        // UPDATE ///////////////////////////////////////////////


        // DRAW /////////////////////////////////////////////////
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // 3D ---------------------------------------------------
        BeginMode3D(camera);

        robot.draw();

        if (menu.flags.show_axes) {
            robot.draw_axes(0b101010);
            draw_axes_3d(0.01, MatrixIdentity());
        }

        DrawGrid(10, 0.25f);

        EndMode3D();
        // 3D ---------------------------------------------------
        
        menu.draw();

        if (ask_to_quit_box) {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
            int result = GuiMessageBox(
                (Rectangle){ (float)GetScreenWidth()/2 - 125, (float)GetScreenHeight()/2 - 50, 250, 100 },
                GuiIconText(ICON_EXIT, "Close Window"),
                "Do you really want to exit?", "Yes;No");
            if ((result == 0) || (result == 2)) {
                ask_to_quit_box = false;
            } else if (result == 1) {
                exit_window = true;
            }
        }

        EndDrawing();
        // DRAW /////////////////////////////////////////////////
    }

    return 0;
}
