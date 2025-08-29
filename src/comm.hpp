#pragma once
#include "comm_base.hpp"
#include "ur_rtde/rtde_receive_interface.h"
#include <pv/caProvider.h>
#include <pva/client.h>

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

class EPICSConnMon : public pvac::ClientChannel::ConnectCallback {
  public:
    EPICSConnMon() = default;
    virtual ~EPICSConnMon() override = default;
    virtual void connectEvent(const pvac::ConnectEvent &event) override final;
    bool connected_ = false;
};

class UREpicsComm : public URCommBase {
  public:
    UREpicsComm();
    // ~UREpicsComm();
    bool connect(const std::string &ioc_prefix) override;
    void disconnect() override;
    bool connected() override;
    RobotState get_robot_state() override;

  private:
    std::string prefix_;
    std::unique_ptr<pvac::ClientProvider> provider_;
    std::unique_ptr<pvac::MonitorSync> monitor_;
    std::unique_ptr<pvac::ClientChannel> channel_;
    std::unique_ptr<EPICSConnMon> connection_monitor_;
    std::vector<float> last_angles_ = std::vector<float>(6, 0.0f);
    bool connected_ = false;
};
