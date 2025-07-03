#include <string>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "rl_utils.hpp"
#include "ur.hpp"
#include "ui.hpp"
#include "i_comm.hpp"
#include "comm.hpp"

std::unique_ptr<IURCommunication> ur_comm;

int main(void) {

    // initialize the window
    SetTraceLogLevel(LOG_FATAL);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    RLWindow window(1400, 950, "UR Robot Viewer");
    
    std::filesystem::path resource_dir = URVIEWER_RESOURCE_DIR;
    Font font = LoadFontEx((resource_dir / "fonts/JetBrainsMonoNerdFont-Regular.ttf").c_str(), 24, 0, 0);
    GuiSetFont(font);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

    // Define the camera to look into our 3d world
    Camera camera = {0};
    camera.position = Vector3{-1.0f, 1.0f, -1.25f}; // Camera position
    camera.target = Vector3{0.0f, 0.25f, 0.0f};  // Camera looking at point
    camera.up = Vector3{0.0f, 1.0f, 0.0f};       // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                         // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;      // Camera mode type

    // Load models and apply initial transforms
    UR robot_model(URVersion::UR3e);

    // Connection to the robot
    // TODO: support EPICS backend
    ur_comm = std::make_unique<URRtdeComm>();

    // For rendering the UI and keeping track of its state
    Ui ui;

    // Used to store data from the robot
    RobotState robot_state;

    while (!ui.state.exit_window) {

        // UPDATE ///////////////////////////////////////////////
        // ask if user really wants to exit
        if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) {
            ui.state.ask_to_quit = !ui.state.ask_to_quit;
        }

        // for adjusting the camera
        if (IsKeyDown(KEY_LEFT_CONTROL) or IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        }

        // get data from robot
        robot_state = ur_comm->get_robot_state();
        if (robot_state.connected) {
            robot_model.update(robot_state.joint_angles);
        }

        // update the UI, provide current robot joint angles
        ui.update(robot_state);

        // Connect/disconnect to robot when requested
        if (ui.state.connect_called) {
            ur_comm->connect(ui.state.conn_string);
            ui.state.connect_called = false;
        } else if (ui.state.disconnect_called) {
            ur_comm->disconnect();
            ui.state.disconnect_called = false;
        }
        // UPDATE ///////////////////////////////////////////////


        // DRAW /////////////////////////////////////////////////
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // 3D ---------------------------------------------------
        BeginMode3D(camera);

        robot_model.draw(ui.state.wires_mask, !robot_state.connected);
        robot_model.draw_axes(ui.state.axes_mask);

        DrawGrid(10, 0.25f);

        EndMode3D();
        // 3D ---------------------------------------------------
        
        ui.draw();

        EndDrawing();
        // DRAW /////////////////////////////////////////////////
    }

    return 0;
}
