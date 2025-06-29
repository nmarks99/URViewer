#include <string>
#include <filesystem>

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "ur_rtde/rtde_receive_interface.h"

#include "utils.hpp"
#include "ur.hpp"

int main(void) {

    // cmake defines the URVIEWER_MODEL_DIR macro
    std::filesystem::path model_dir = URVIEWER_MODEL_DIR;

    // initialize the window
    SetTraceLogLevel(LOG_FATAL);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    RLWindow window(800, 800, "UR Robot Viewer");

    // Define the camera to look into our 3d world
    Camera camera = {0};
    camera.position = Vector3{1.0f, 1.0f, 1.0f}; // Camera position
    camera.target = Vector3{0.0f, 0.25f, 0.0f};  // Camera looking at point
    camera.up = Vector3{0.0f, 1.0f, 0.0f};       // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                         // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;      // Camera mode type

    // Load models and apply initial transforms
    auto robot = UR(URVersion::UR3e);

    // Values to store joint angles in radians
    float j1 = 0.0;
    float j2 = 0.0;
    float j3 = 0.0;
    float j4 = 0.0;
    float j5 = 0.0;
    float j6 = 0.0;

    bool show_axes = false;

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_LEFT_CONTROL) or IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        }

        // update transforms for robot given joint angles
        robot.update({j1,j2,j3,j4,j5,j6});

        // DRAW /////////////////////////////////
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // sliders to adjust joint angles
        GuiSlider((Rectangle){50, 20, 216, 16}, TextFormat("%0.2f", j1), NULL, &j1, -PI, PI);
        GuiSlider((Rectangle){50, 40, 216, 16}, TextFormat("%0.2f", j2), NULL, &j2, -PI, PI);
        GuiSlider((Rectangle){50, 60, 216, 16}, TextFormat("%0.2f", j3), NULL, &j3, -PI, PI);
        GuiSlider((Rectangle){50, 80, 216, 16}, TextFormat("%0.2f", j4), NULL, &j4, -PI, PI);
        GuiSlider((Rectangle){50, 100, 216, 16}, TextFormat("%0.2f", j5), NULL, &j5, -PI, PI);
        GuiSlider((Rectangle){50, 120, 216, 16}, TextFormat("%0.2f", j6), NULL, &j6, -PI, PI);

        // check box to show/hide axes
        GuiCheckBox((Rectangle){ static_cast<float>(GetScreenWidth()-100.0), 20, 15, 15 }, "Show axes", &show_axes);

        // 3D -------------
        BeginMode3D(camera);

        robot.draw();

        if (show_axes) {
            robot.draw_axes();
            draw_axes_3d(0.01, MatrixIdentity());
        }

        DrawGrid(20, 0.25f);

        EndMode3D();
        // 3D -------------

        EndDrawing();
        // DRAW /////////////////////////////////
    }

    return 0;
}
