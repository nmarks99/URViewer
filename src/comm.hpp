#pragma once
#include "i_comm.hpp"
#include "ur_rtde/rtde_receive_interface.h"

class URRtdeComm : public IURCommunication{
  public:
    URRtdeComm();
    ~URRtdeComm();
    bool connect(const std::string &robot_ip);
    void disconnect();
    bool connected();
    // std::vector<float> get_joint_angles();
    RobotState get_robot_state();

  private:
    std::unique_ptr<ur_rtde::RTDEReceiveInterface> recv_;
};

// class UREpicsComm : public IURCommunication{
  // public:
    // UREpicsComm();
    // ~UREpicsComm();
    // bool connect(const std::string &robot_ip);
    // void disconnect();
    // bool connected();
    // std::vector<float> get_joint_angles();
// };
