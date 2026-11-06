#include "comm.hpp"

#include <algorithm>

URRtdeComm::URRtdeComm() : recv_(nullptr) {}

URRtdeComm::~URRtdeComm() {
    if (recv_) {
        recv_->disconnect();
    }
}

bool URRtdeComm::connect(const std::string &robot_ip) {
    bool connected = false;
    try {
        recv_ = std::make_unique<ur_rtde::RTDEReceiveInterface>(robot_ip);
        if (recv_) {
            if (recv_->isConnected()) {
                connected = true;
            }
        }
        return true;
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        connected = false;
    }
    return connected;
}

void URRtdeComm::disconnect() {
    if (recv_) {
        recv_->disconnect();
    }
}

bool URRtdeComm::connected() {
    if (recv_) {
        return recv_->isConnected();
    } else {
        return false;
    }
}

RobotState URRtdeComm::get_robot_state() {
    std::vector<double> qvec_double(6);
    std::vector<float> qvec_float(6);
    bool connected = false;
    if (recv_) {
        if (recv_->isConnected()) {
            connected = true;

            qvec_double = recv_->getActualQ();
            for (int i = 0; i < qvec_double.size(); i++) {
                qvec_float.at(i) = static_cast<float>(qvec_double.at(i));
            }
        }
    }
    return RobotState{
        .connected = connected,
        .joint_angles = qvec_float,
    };
}

inline const std::string JOINT_ANGLES_PV_NAME = "Receive:ActualJointPositions";

void UREpicsComm::disconnect() {
    joint_angles_channel_ = nullptr;
    context_.reset();
    joint_angles_degrees_.clear();
    std::fill(joint_angles_radians_.begin(), joint_angles_radians_.end(), 0.0f);
}

bool UREpicsComm::connected() {
    return joint_angles_channel_ && joint_angles_channel_->connected();
}

bool UREpicsComm::connect(const std::string &ioc_prefix) {
    disconnect();

    const std::string pv_name = std::string(ioc_prefix.c_str()) + JOINT_ANGLES_PV_NAME;
    context_ = std::make_unique<pace::Context>();
    joint_angles_channel_ = &context_->connect(pv_name);
    context_->bind(joint_angles_degrees_, pv_name);
    return true;
}

RobotState UREpicsComm::get_robot_state() {
    if (!context_ || !connected()) {
        return RobotState{.connected = false};
    }

    if (context_->sync() && joint_angles_degrees_.size() == joint_angles_radians_.size()) {
        for (size_t i = 0; i < joint_angles_degrees_.size(); i++) {
            joint_angles_radians_[i] = static_cast<float>(joint_angles_degrees_[i] * (M_PI / 180.0));
        }
    }

    return RobotState{.connected = true, .joint_angles = joint_angles_radians_};
}
