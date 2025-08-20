#pragma once
#include "comm_base.hpp"
#include "ur_rtde/rtde_receive_interface.h"
#include <pva/client.h>
#include <pv/caProvider.h>

class URRtdeComm : public IURCommunication{
  public:
    URRtdeComm();
    ~URRtdeComm();
    bool connect(const std::string &robot_ip);
    void disconnect();
    bool connected();
    RobotState get_robot_state();

  private:
    std::unique_ptr<ur_rtde::RTDEReceiveInterface> recv_;
};

class UREpicsComm : public IURCommunication{
  public:
    UREpicsComm();
    bool connect(const std::string &ioc_prefix) override;
    void disconnect() override;
    bool connected() override;
    RobotState get_robot_state() override;

  private:
    std::string prefix_;
    std::unique_ptr<pvac::ClientProvider> provider_;
    std::unique_ptr<pvac::ClientChannel> channel_;
    bool connected_ = false;
};
