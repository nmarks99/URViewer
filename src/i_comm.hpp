#pragma once
#include <vector>
#include <string>

struct RobotState {
    bool connected = false;
    std::vector<float> joint_angles = std::vector<float>(6, 0.0f);
};

class IURCommunication {
  public:
    virtual bool connect(const std::string &conn_string) = 0;
    virtual void disconnect() = 0;
    virtual bool connected() = 0;
    virtual RobotState get_robot_state() = 0;
    virtual ~IURCommunication() = default;
};
