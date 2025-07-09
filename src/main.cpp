#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "rl_utils.hpp"
#include "ur.hpp"
#include "ui.hpp"
#include "i_comm.hpp"
#include "comm.hpp"

std::unique_ptr<IURCommunication> ur_comm;

int main(int argc, char *argv[]) {

    // Set robot model from command line arg
    URVersion ur_version = URVersion::UR3e;
    if (argc > 1) {
        std::string_view arg = argv[1];
        if (arg.find("UR3") != std::string_view::npos) {
            ur_version = URVersion::UR3e;
        } else if (arg.find("UR5") != std::string_view::npos) {
            ur_version = URVersion::UR5e;
        } else {
            std::cerr << "Unknown robot model in argument: '" << arg << "'\n";
            return EXIT_FAILURE;
        }
    }

    // initialize the window
    RLWindow window(1400, 950, "UR Robot Viewer");

    // Set up 3D camera
    RLCamera3D cam;

    // For rendering the UI and keeping track of its state
    Ui ui;

    // Load models and apply initial transforms
    UR robot_model(ur_version);
    robot_model.load();

    // Connection to the robot
    // There may be additional backends besides URRtdeComm in the future
    ur_comm = std::make_unique<URRtdeComm>();

    // Used to store data from the robot
    RobotState robot_state;

    while (!ui.state.exit_window) {

        // UPDATE ///////////////////////////////////////////////
        cam.update();

        if (ui.state.connect_called) {
            ur_comm->connect(ui.state.conn_string);
            ui.state.connect_called = false;
        } else if (ui.state.disconnect_called) {
            ur_comm->disconnect();
            ui.state.disconnect_called = false;
        }

        robot_state = ur_comm->get_robot_state();
        if (robot_state.connected) {
            robot_model.update(robot_state.joint_angles);
        }

        ui.update(robot_state);
        // UPDATE ///////////////////////////////////////////////


        // DRAW /////////////////////////////////////////////////
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // 3D ---------------------------------------------------
        BeginMode3D(cam.camera);

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
