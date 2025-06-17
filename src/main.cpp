#include "raylib.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

void draw_world_axes();
Vector3 extract_translation(const Matrix &m);

struct Link {
    Model model;

    Link(const char *model_path);
    ~Link();

    void draw();
    void draw_axes();
};

struct Window {
    Window(int width, int height, const char *title) {
        InitWindow(width, height, title);
    };

    ~Window() {
        TraceLog(LOG_INFO, "Closing window\n");
        CloseWindow();
    }
};


int main(void) {

    SetTraceLogLevel(LOG_FATAL);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    Window window(800, 800, "UR Robot Viewer");

    // Define the camera to look into our 3d world
    Camera camera = {0};
    camera.position = Vector3{1.0f, 1.0f, 1.0f}; // Camera position
    camera.target = Vector3{0.0f, 0.0f, 0.0f};   // Camera looking at point
    camera.up = Vector3{0.0f, 1.0f, 0.0f};       // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                         // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;      // Camera mode type

    // Load models
    Link base("models/Base_UR3_G5_12.gltf");
    base.model.transform = MatrixMultiply(MatrixIdentity(), MatrixTranslate(0.25, 0.0, 0.0));

    Link link1("models/Link1_UR3_G5.gltf");
    link1.model.transform = MatrixMultiply(MatrixIdentity(), MatrixRotateY(0.7));
    link1.model.transform = MatrixMultiply(link1.model.transform, MatrixTranslate(-0.25, 0.0, 0.0));

    Link link2("models/Link2_UR3_G5_2.gltf");
    link2.model.transform = MatrixMultiply(MatrixIdentity(), MatrixRotateY(-0.7));
    link2.model.transform = MatrixMultiply(link2.model.transform, MatrixTranslate(0.25, 0.25, 0.0));

    Vector3 link2_trans_vec = extract_translation(link2.model.transform);
    Matrix link2_trans = MatrixTranslate(link2_trans_vec.x, link2_trans_vec.y, link2_trans_vec.z);


    float link2_angle_y = 0.0;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        }

        link2.model.transform = MatrixMultiply(MatrixRotateY(link2_angle_y), link2_trans);

        // DRAW /////////////////////////////////
        BeginDrawing();
        ClearBackground(RAYWHITE);

        GuiSlider((Rectangle){ 96, 48, 216, 16 }, TextFormat("%0.2f", link2_angle_y), NULL, &link2_angle_y, -2*3.14f, 2*3.14f);

        // 3D -------------
        BeginMode3D(camera);

        base.draw();
        base.draw_axes();

        link1.draw();
        link1.draw_axes();

        link2.draw();
        link2.draw_axes();

        draw_world_axes();
        DrawGrid(20, 0.25f);

        EndMode3D();
        // 3D -------------

        EndDrawing();
        // DRAW /////////////////////////////////
    }

    return 0;
}

void draw_world_axes() {
    const float thickness = 0.005;
    const Vector3 xvec{0.1, 0.0, 0.0};
    const Vector3 yvec{0.0, 0.1, 0.0};
    const Vector3 zvec{0.0, 0.0, 0.1};
    const Vector3 position{0.0, 0.0, 0.0};
    const int sides = 20;
    DrawCylinderEx(position, Vector3Add(position, xvec), thickness, thickness, sides, RED);
    DrawCylinderEx(position, Vector3Add(position, yvec), thickness, thickness, sides, GREEN);
    DrawCylinderEx(position, Vector3Add(position, zvec), thickness, thickness, sides, BLUE);
}

Vector3 extract_translation(const Matrix &m) {
    return Vector3 {
        .x = m.m12,
        .y = m.m13,
        .z = m.m14
    };
}

Link::Link(const char *model_path) : model(LoadModel(model_path)) {}

Link::~Link() {
    if (model.meshCount > 0) {
        TraceLog(LOG_INFO, "Unloading model\n");
        UnloadModel(model);
    }
}

void Link::draw() {
    DrawModel(model, Vector3Zeros, 1.0, WHITE);
}

void Link::draw_axes() {
    const float thickness = 0.005;
    const int sides = 20;
    auto position = Vector3Transform(Vector3Zeros, model.transform);
    auto x_rot_vec = Vector3Transform({0.1,0.0,0.0}, model.transform);
    auto y_rot_vec = Vector3Transform({0.0,0.1,0.0}, model.transform);
    auto z_rot_vec = Vector3Transform({0.0,0.0,0.1}, model.transform);
    DrawCylinderEx(position, x_rot_vec, thickness, thickness, sides, RED);
    DrawCylinderEx(position, y_rot_vec, thickness, thickness, sides, GREEN);
    DrawCylinderEx(position, z_rot_vec, thickness, thickness, sides, BLUE);
}
