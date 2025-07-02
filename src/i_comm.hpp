#pragma once
#include <vector>
#include <string>

class IURCommunication {
  public:
    virtual bool connect(const std::string &conn_string) = 0;
    virtual void disconnect() = 0;
    virtual bool connected() = 0;
    virtual std::vector<float> get_joint_angles() = 0;
    virtual ~IURCommunication() = default;
};
