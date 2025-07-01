#include "comm.hpp"

URRtdeSource::URRtdeSource() : recv_(nullptr) {
}

URRtdeSource::~URRtdeSource() {
    if (recv_) {
        recv_->disconnect();
    }
}

void URRtdeSource::connect(const std::string &robot_ip) {
    try {
        recv_ = std::make_unique<ur_rtde::RTDEReceiveInterface>(robot_ip);
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

bool URRtdeSource::connected() {
    if (recv_) {
        return recv_->isConnected();
    } else {
        return false;
    }
}

std::vector<float> URRtdeSource::get_joint_angles() {
    std::vector<double> qvec_double(6);
    std::vector<float> qvec_float(6);
    if (recv_) {
        qvec_double = recv_->getActualQ();
        for (int i = 0; i < qvec_double.size(); i++) {
            qvec_float.at(i) = static_cast<float>(qvec_double.at(i));
        }
    }
    return qvec_float;
}
