#include <string>
#include <filesystem>

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "ur_rtde/rtde_receive_interface.h"

#include "UR3e.hpp"
#include "utils.hpp"


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
    RLModel base((model_dir / "obj/base.obj").string().c_str());
    base.model.transform = TSBASE;

    RLModel link1((model_dir / "obj/shoulder.obj").string().c_str());
    link1.model.transform = MatrixMultiply(TB1, base.model.transform);

    RLModel link2((model_dir / "obj/upperarm.obj").string().c_str());
    link2.model.transform = MatrixMultiply(T12, link1.model.transform);

    RLModel link3((model_dir / "obj/forearm.obj").string().c_str());
    link3.model.transform = MatrixMultiply(T23, link2.model.transform);

    RLModel link4((model_dir / "obj/wrist1.obj").string().c_str());
    link4.model.transform = MatrixMultiply(T34, link3.model.transform);

    RLModel link5((model_dir / "obj/wrist2.obj").string().c_str());
    link5.model.transform = MatrixMultiply(T45, link4.model.transform);

    RLModel link6((model_dir / "obj/wrist3.obj").string().c_str());
    link6.model.transform = MatrixMultiply(T56, link5.model.transform);

    // Values to store joint angles in radians
    float joint1 = 0.0;
    float joint2 = 0.0;
    float joint3 = 0.0;
    float joint4 = 0.0;
    float joint5 = 0.0;
    float joint6 = 0.0;

    bool show_axes = false;

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_LEFT_CONTROL) or IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        }

        // update transforms based on joint changes
        link1.model.transform = MatrixMultiply(MatrixMultiply(MatrixRotateY(joint1), TB1), base.model.transform);
        link2.model.transform = MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint2), T12), link1.model.transform);
        link3.model.transform = MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint3), T23), link2.model.transform);
        link4.model.transform = MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint4), T34), link3.model.transform);
        link5.model.transform = MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint5), T45), link4.model.transform);
        link6.model.transform = MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint6), T56), link5.model.transform);

        // DRAW /////////////////////////////////
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // sliders to adjust joint angles
        GuiSlider((Rectangle){50, 20, 216, 16}, TextFormat("%0.2f", joint1), NULL, &joint1, -PI, PI);
        GuiSlider((Rectangle){50, 40, 216, 16}, TextFormat("%0.2f", joint2), NULL, &joint2, -PI, PI);
        GuiSlider((Rectangle){50, 60, 216, 16}, TextFormat("%0.2f", joint3), NULL, &joint3, -PI, PI);
        GuiSlider((Rectangle){50, 80, 216, 16}, TextFormat("%0.2f", joint4), NULL, &joint4, -PI, PI);
        GuiSlider((Rectangle){50, 100, 216, 16}, TextFormat("%0.2f", joint5), NULL, &joint5, -PI, PI);
        GuiSlider((Rectangle){50, 120, 216, 16}, TextFormat("%0.2f", joint6), NULL, &joint6, -PI, PI);

        // check box to show/hide axes
        GuiCheckBox((Rectangle){ static_cast<float>(GetScreenWidth()-100.0), 20, 15, 15 }, "Show axes", &show_axes);

        // 3D -------------
        BeginMode3D(camera);

        // Draw all the models
        base.draw();
        link1.draw();
        link2.draw();
        link3.draw();
        link4.draw();
        link5.draw();
        link6.draw();

        // show model and world axes if requested
        if (show_axes) {
            base.draw_axes();
            link1.draw_axes();
            link2.draw_axes();
            link3.draw_axes();
            link4.draw_axes();
            link5.draw_axes();
            link6.draw_axes();
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
