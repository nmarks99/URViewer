#pragma once
#include "comm_base.hpp"
#include "pace.hpp"
#include "ur_rtde/rtde_receive_interface.h"

#include <memory>

class URRtdeComm : public URCommBase {
  public:
    URRtdeComm();
    ~URRtdeComm();
    bool connect(const std::string &robot_ip) override;
    void disconnect() override;
    bool connected() override;
    RobotState get_robot_state() override;

  private:
    std::unique_ptr<ur_rtde::RTDEReceiveInterface> recv_;
};

class UREpicsComm : public URCommBase {
  public:
    bool connect(const std::string &ioc_prefix) override;
    void disconnect() override;
    bool connected() override;
    RobotState get_robot_state() override;

  private:
    std::vector<double> joint_angles_degrees_;
    std::vector<float> joint_angles_radians_ = std::vector<float>(6, 0.0f);
    pace::ChannelBase *joint_angles_channel_ = nullptr;
    std::unique_ptr<pace::Context> context_;
};
