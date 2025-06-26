#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "UR3e.hpp"

void draw_world_axes();

struct Link {
  public:
    Model model;

    Link(const char *model_path);
    ~Link();
    Link& operator=(const Link&) = delete;

    void draw();
    void draw_wires();
    void draw_axes();
};

struct Window {
    Window(int width, int height, const char *title) {
        InitWindow(width, height, title);
    };

    ~Window() {
        CloseWindow();
    }

    Window& operator=(const Window&) = delete;
};


int main(void) {

    SetTraceLogLevel(LOG_FATAL);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    Window window(800, 800, "UR Robot Viewer");

    // Define the camera to look into our 3d world
    Camera camera = {0};
    camera.position = Vector3{1.0f, 1.0f, 1.0f}; // Camera position
    camera.target = Vector3{0.0f, 0.25f, 0.0f};  // Camera looking at point
    camera.up = Vector3{0.0f, 1.0f, 0.0f};       // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                         // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;      // Camera mode type

    SetTargetFPS(60);

    // Load models and apply initial transforms
    Link base("models/base.gltf");
    base.model.transform = TSBASE;

    Link link1("models/shoulder.gltf");
    link1.model.transform = MatrixMultiply(TB1, base.model.transform);

    Link link2("models/upperarm.gltf");
    link2.model.transform = MatrixMultiply(T12, link1.model.transform);

    Link link3("models/forearm.gltf");
    link3.model.transform = MatrixMultiply(T23, link2.model.transform);

    Link link4("models/wrist1.gltf");
    link4.model.transform = MatrixMultiply(T34, link3.model.transform);

    Link link5("models/wrist2.gltf");
    link5.model.transform = MatrixMultiply(T45, link4.model.transform);

    Link link6("models/wrist3.gltf");
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
            draw_world_axes();
        }

        DrawGrid(20, 0.25f);

        EndMode3D();
        // 3D -------------

        EndDrawing();
        // DRAW /////////////////////////////////
    }

    return 0;
}

void draw_world_axes() {
    const float thickness = 0.010;
    const Vector3 xvec{0.1, 0.0, 0.0};
    const Vector3 yvec{0.0, 0.1, 0.0};
    const Vector3 zvec{0.0, 0.0, 0.1};
    const Vector3 position{0.0, 0.0, 0.0};
    const int sides = 20;
    DrawCylinderEx(position, Vector3Add(position, xvec), thickness, thickness, sides, RED);
    DrawCylinderEx(position, Vector3Add(position, yvec), thickness, thickness, sides, GREEN);
    DrawCylinderEx(position, Vector3Add(position, zvec), thickness, thickness, sides, BLUE);
}

Link::Link(const char *model_path) : model(LoadModel(model_path)) {}

Link::~Link() {
    if (model.meshCount > 0) {
        TraceLog(LOG_INFO, "Unloading model\n");
        UnloadModel(model);
    }
}

void Link::draw() { DrawModel(model, Vector3Zeros, 1.0, WHITE); }

void Link::draw_wires() { DrawModelWires(model, Vector3Zeros, 1.0, WHITE); }

void Link::draw_axes() {
    const float thickness = 0.005;
    const int sides = 20;
    auto origin = Vector3Transform(Vector3Zeros, model.transform);
    auto x_end = Vector3Transform({0.1, 0.0, 0.0}, model.transform);
    auto y_end = Vector3Transform({0.0, 0.1, 0.0}, model.transform);
    auto z_end = Vector3Transform({0.0, 0.0, 0.1}, model.transform);
    DrawCylinderEx(origin, x_end, thickness, thickness, sides, RED);
    DrawCylinderEx(origin, y_end, thickness, thickness, sides, GREEN);
    DrawCylinderEx(origin, z_end, thickness, thickness, sides, BLUE);
}
