#pragma once
#include <vector>
#include <string>
#include "ur_rtde/rtde_receive_interface.h"

// class URDataSource {
  // public:
    // virtual void connect() = 0;
    // virtual bool connected() = 0;
    // virtual std::vector<float> get_joint_angles() = 0;
    // ~URDataSource() = default;
// };

class URRtdeSource{
  public:
    URRtdeSource();
    ~URRtdeSource();
    void connect(const std::string &robot_ip);
    bool connected();
    std::vector<float> get_joint_angles();

  private:
    std::unique_ptr<ur_rtde::RTDEReceiveInterface> recv_;
};
