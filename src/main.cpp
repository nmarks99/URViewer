#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "argh.h"
#include "comm.hpp"
#include "comm_base.hpp"
#include "rl_utils.hpp"
#include "ui.hpp"
#include "ur.hpp"

const std::string CLI_HELP_MESSAGE = R"(
URViewer - Live 3D viewer for Universal Robots

Usage:
  URViewer [options]

Options:
  -h, --help          Show this help message and exit
  --model <model>     Robot model to use (UR3, UR5)
  --backend <backend> Communication backend (EPICS, TCP)
  --IP <ip_address>   IP address of the robot controller (if using TCP/IP backend)
  --prefix <prefix>   IOC prefix (if using EPICS backend)

Examples:
  # Start URViewer with UR3 model and given IP
  ./URViewer --model UR3 --IP 192.168.1.100

  # Start URViewer with UR3 model and given EPICS IOC prefix
  ./URViewer --model UR3 --backend EPICS --prefix 192.168.1.100

  # Start URViewer with defaults:
  # TCP/IP backend, UR3 model, IP must be entered in GUI
  ./URViewer
)";

std::unique_ptr<URCommBase> ur_comm;

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
        backend = (backend_arg_str == "EPICS") ? CommBackend::EPICS : CommBackend::TCPIP;
    }

    // initialize the window
    // RLWindow window(1400, 950, "UR Robot Viewer");
    RLWindow window(1120, 760, "UR Robot Viewer");

    // Set up 3D camera
    RLCamera3D cam;

    // For rendering the UI and keeping track of its state
    UIState ui_state0;
    std::copy(connection_string.begin(), connection_string.end(), ui_state0.connection_string.begin());
    Ui ui(ui_state0);

    // Load models and apply initial transforms
    UR robot_model(ur_version);

    // Connection to the robot
    if (backend == CommBackend::EPICS) {
        ur_comm = std::make_unique<UREpicsComm>();
    } else if (backend == CommBackend::TCPIP) {
        ur_comm = std::make_unique<URRtdeComm>();
    }

    // Used to store data from the robot
    RobotState robot_state;

    while (!ui.state.exit_window) {

        // UPDATE ///////////////////////////////////////////////
        if (!ui.state.conn_text_active) {
            cam.update();
        }

        CommBackend selected_backend =
            (ui.state.backend_menu_selected == 0) ? CommBackend::TCPIP : CommBackend::EPICS;
        if (selected_backend != backend) {
            backend = selected_backend;
            ur_comm->disconnect();
            if (selected_backend == CommBackend::TCPIP) {
                ur_comm = std::make_unique<URRtdeComm>();
            } else {
                ur_comm = std::make_unique<UREpicsComm>();
            }
        }

        URVersion selected_ur_version =
            (ui.state.model_menu_selected == 0) ? URVersion::UR3e : URVersion::UR5e;
        if (selected_ur_version != ur_version) {
            ur_version = selected_ur_version;
            robot_model.unload();
            robot_model.load(selected_ur_version);
        }

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
