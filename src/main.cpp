#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "rl_utils.hpp"
#include "ur.hpp"
#include "ui.hpp"
#include "comm_base.hpp"
#include "comm.hpp"
#include "argh.h"

const std::string CLI_HELP_MESSAGE = R"(
URViewer - Live 3D viewer for Universal Robots

Usage:
  URViewer [options]

Options:
  -h, --help    Show this help message and exit.
  --model       Robot model (UR3 or UR5)
  --IP          IP address of the robot controller

Examples:
    # start URViewer with UR3 model and given IP
    ./URViewer --model UR3 --IP 192.168.1.100

    # start URViewer with default model (UR3)
    # Enter IP address in GUI
    ./URViewer
)";

std::unique_ptr<IURCommunication> ur_comm;

int main(int argc, char *argv[]) {

    // Parse command line arguments
    argh::parser args;
    args.add_params({"--model"});
    args.add_params({"--backend"});
    args.add_params({"--IP", "--ip", "--prefix"});
    args.parse(argc, argv);

    if (args["-h"] or args["--help"]) {
        std::cout << CLI_HELP_MESSAGE << std::endl;
        return EXIT_SUCCESS;
    }

    URVersion ur_version = URVersion::UR3e;
    std::string connection_string = "";
    CommBackend backend;

    if (args.size()) {
        const std::string model_arg_str = args({"--model"}).str();
        if (model_arg_str.size()) {
            if (model_arg_str == "UR3" or model_arg_str == "UR3e") {
                ur_version = URVersion::UR3e;
            } else if (model_arg_str == "UR5" or model_arg_str == "UR5e") {
                ur_version = URVersion::UR5e;
            } else {
                std::cerr << "Unknown UR model " << model_arg_str << std::endl;
                return EXIT_FAILURE;
            }
        }
        const std::string conn_arg_str = args({"--ip", "--IP", "--prefix"}).str();
        connection_string = conn_arg_str.size() ? conn_arg_str : "";

        const std::string backend_arg_str = args({"--backend"}).str();
        backend = (backend_arg_str == "EPICS") ? CommBackend::EPICS : CommBackend::RTDE;
    }

    // initialize the window
    RLWindow window(1400, 950, "UR Robot Viewer");

    // Set up 3D camera
    RLCamera3D cam;

    // For rendering the UI and keeping track of its state
    UIState ui_state;
    std::copy(connection_string.begin(), connection_string.end(), ui_state.connection_string.begin());
    std::cout << "using connection string " << ui_state.connection_string << "\n";
    Ui ui(ui_state);

    // Load models and apply initial transforms
    UR robot_model(ur_version);
    robot_model.load();

    // Connection to the robot
    if (backend == CommBackend::EPICS) {
        ur_comm = std::make_unique<UREpicsComm>();
    } else if (backend == CommBackend::RTDE) {
        ur_comm = std::make_unique<URRtdeComm>();
    }

    // Used to store data from the robot
    RobotState robot_state;

    while (!ui.state.exit_window) {

        // UPDATE ///////////////////////////////////////////////
        cam.update();

        if (ui.state.connect_called) {
            ur_comm->connect(ui.state.connection_string);
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
