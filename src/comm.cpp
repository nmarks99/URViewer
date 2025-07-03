#include "comm.hpp"

URRtdeComm::URRtdeComm() : recv_(nullptr) {
}

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

// std::vector<float> URRtdeComm::get_joint_angles() {
    // std::vector<double> qvec_double(6);
    // std::vector<float> qvec_float(6);
    // if (recv_) {
        // qvec_double = recv_->getActualQ();
        // for (int i = 0; i < qvec_double.size(); i++) {
            // qvec_float.at(i) = static_cast<float>(qvec_double.at(i));
        // }
    // }
    // return qvec_float;
// }

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
