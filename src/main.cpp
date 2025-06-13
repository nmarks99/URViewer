#include "raylib.h"
#include "raymath.h"

struct Entity {

    Entity(Model model) : model(model) {}
    Entity(Model model, Vector3 position) : model(model), position(position) {}
    Entity(Model model, Vector3 position, Color color) : model(model), position(position), color(color) {}
    
    Model model;
    Vector3 position = {0.0, 0.0, 0.0};
    float scale = 1.0;
    Color color = WHITE;

    void draw() {
        DrawModel(model, position, scale, color);
    }

    void draw_coord_axes() {
        const float axis_thickness = 0.005;
        Vector3 xvec{0.1, 0.0, 0.0};
        Vector3 yvec{0.0, 0.1, 0.0};
        Vector3 zvec{0.0, 0.0, 0.1};
        DrawCylinderEx(position, Vector3Add(position, xvec), axis_thickness, axis_thickness, 20, RED);
        DrawCylinderEx(position, Vector3Add(position, yvec), axis_thickness, axis_thickness, 20, GREEN);
        DrawCylinderEx(position, Vector3Add(position, zvec), axis_thickness, axis_thickness, 20, BLUE);
    }
};

int main(void) {
    const int screenWidth = 1000;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "UR Robot Viewer");

    // Define the camera to look into our 3d world
    Camera camera = {0};
    camera.position = (Vector3){1.0f, 1.0f, 1.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};   // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};       // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                           // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;        // Camera mode type

    Model base_model = LoadModel("models/Base_UR3_G5_12.gltf");
    Entity base = Entity(base_model, {0.0, 0.0, 0.25});

    Model link1_model = LoadModel("models/Link1_UR3_G5.gltf");
    Entity link1 = Entity(link1_model, {0.0, 0.0, -0.25});
    
    Model link2_model = LoadModel("models/Link2_UR3_G5_2.gltf");
    Entity link2 = Entity(link2_model, {0.0, 0.0, 0.0});

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) {
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                base.draw();
                base.draw_coord_axes();
                link1.draw();
                link1.draw_coord_axes();
                // link2.draw();
                link2.draw_coord_axes();
                DrawModelEx(link2.model, link2.position, {0.0, 0.0, 1.0}, -90.0, {1,1,1}, WHITE);

                DrawGrid(20, 0.25f); // Draw a grid

            EndMode3D();

            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadModel(base_model);
    UnloadModel(link1_model);
    UnloadModel(link2_model);
    CloseWindow();

    return 0;
}
