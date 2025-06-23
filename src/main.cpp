#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "UR3e.hpp"

constexpr Matrix Rx(float th) {
    using namespace std;
    return Matrix {
        1.0,      0.0,     0.0, 0.0,
        0.0,  cos(th), sin(th), 0.0,
        0.0, -sin(th), cos(th), 0.0,
        0.0,      0.0,     0.0, 1.0,
    };
}

constexpr Matrix Ry(float th) {
    using namespace std;
     return Matrix {
        cos(th), 0.0, -sin(th), 0.0,
        sin(th), 1.0,  cos(th), 0.0,
            0.0, 0.0,      0.0, 0.0,
            0.0, 0.0,      0.0, 1.0,
    };
}

constexpr Matrix Rz(float th) {
    using namespace std;
     return Matrix {
        cos(th), -sin(th), 0.0, 0.0,
        sin(th),  cos(th), 0.0, 0.0,
            0.0,      0.0, 1.0, 0.0,
            0.0,      0.0, 0.0, 1.0,
    };
}


void draw_world_axes();
void print_matrix(const Matrix &m) {
    printf("%f, %f, %f, %f,\n%f, %f, %f, %f,\n%f, %f, %f, %f,\n%f, %f, %f, %f,\n",
           m.m0, m.m4, m.m8, m.m12,
           m.m1, m.m5, m.m9, m.m13,
           m.m2, m.m6, m.m10, m.m14,
           m.m3, m.m7, m.m11, m.m15
    );
}
Vector3 extract_translation(const Matrix &m);

struct Link {
  public:
    Model model;

    Link(const char *model_path);
    ~Link();

    void draw();
    void draw_wires();
    void draw_axes();
};

struct Window {
    Window(int width, int height, const char *title) { InitWindow(width, height, title); };

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

    SetTargetFPS(60);

    // Load models
    Link base("models/Base_UR3.gltf");
    base.model.transform = TSBASE;

    Link link1("models/Link1_UR3.gltf");
    link1.model.transform = TS1;

    Link link2("models/Link2_UR3.gltf");
    link2.model.transform = MatrixMultiply(Rz(-1.57), link1.model.transform);
    link2.model.transform = MatrixMultiply(Rx(3.14), link2.model.transform);
    // link2.model.transform = MatrixMultiply(MatrixRotateZ(-1.57), link1.model.transform);
    // link2.model.transform = MatrixMultiply(MatrixRotateX(3.14), link2.model.transform);
    link2.model.transform = MatrixMultiply(MatrixTranslate(0.0, 0.12, 0.0), link2.model.transform);
    // print_matrix(link2.model.transform);

    Link link3("models/Link3_UR3.gltf");
    link3.model.transform = TS3;
    // link3.model.transform = MatrixMultiply(MatrixRotateX(3.14), link2.model.transform);
    // link3.model.transform = MatrixMultiply(MatrixTranslate(-0.457,0.1,0.0), link3.model.transform);
    // print_matrix(link3.model.transform);

    Link link4("models/Link4_UR3.gltf");
    link4.model.transform = MatrixMultiply(link4.model.transform, MatrixTranslate(0.0, 0.0, -0.25));

    Link link5("models/Link5_UR3.gltf");
    link5.model.transform = MatrixMultiply(link5.model.transform, MatrixTranslate(0.0, 0.0, -0.25));

    Link link6("models/Link6_UR3.gltf");
    link6.model.transform = MatrixMultiply(link6.model.transform, MatrixTranslate(0.5, 0.0, 0.0));

    Vector3 j1_trans_vec = extract_translation(link1.model.transform);
    Matrix j1_trans = MatrixTranslate(j1_trans_vec.x, j1_trans_vec.y, j1_trans_vec.z);
    float joint1 = 0.0;

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        }


        link1.model.transform = MatrixMultiply(MatrixRotateY(joint1), j1_trans); // TS1

        // update the child transforms
        // link2.model.transform = MatrixMultiply(MatrixRotateZYX({PI, 0.0, -PI/2}), link2.model.transform);
        link2.model.transform = MatrixMultiply(MatrixRotateZYX({PI, 0.0, -PI/2}), link1.model.transform);
        link2.model.transform = MatrixMultiply(MatrixTranslate(0.0, 0.12, 0.0), link2.model.transform);

        // DRAW /////////////////////////////////
        BeginDrawing();
        ClearBackground(RAYWHITE);

        GuiSlider((Rectangle){96, 48, 216, 16}, TextFormat("%0.2f", joint1), NULL, &joint1, -2 * 3.14f,
                  2 * 3.14f);
        // GuiSlider((Rectangle){96, 68, 216, 16}, TextFormat("%0.2f", joint1), NULL, &joint1, -2 * 3.14f,
                  // 2 * 3.14f);
        // GuiSlider((Rectangle){96, 88, 216, 16}, TextFormat("%0.2f", rz), NULL, &rz, -2 * 3.14f,
                  // 2 * 3.14f);

        // 3D -------------
        BeginMode3D(camera);

        base.draw();
        base.draw_axes();

        link1.draw();
        link1.draw_axes();

        link2.draw();
        link2.draw_axes();

        link3.draw();
        link3.draw_axes();

        // link4.draw();
        // link4.draw_axes();
//
        // link5.draw();
        // link5.draw_axes();
//
        // link6.draw();
        // link6.draw_axes();

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

Vector3 extract_translation(const Matrix &m) { return Vector3{.x = m.m12, .y = m.m13, .z = m.m14}; }

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
