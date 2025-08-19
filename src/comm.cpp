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

UREpicsComm::UREpicsComm() {
    epics::pvAccess::ca::CAClientFactory::start();
    provider_ = std::make_shared<pvac::ClientProvider>("ca");
}

void UREpicsComm::disconnect() {
    // do nothing
};

bool UREpicsComm::connected() {
    return connected_;
};

bool UREpicsComm::connect(const std::string &ioc_prefix) {
    if (not connected_) {
        std::cout << ioc_prefix + "Receive:ActualJointPositions" << "\n";
        if (provider_) {
            std::cout << "provder_ ok" << "\n";
        }
        channel_ = std::make_unique<pvac::ClientChannel>(provider_.get()->connect(ioc_prefix + "Receive:ActualJointPositions"));
        connected_ = true;
    }
    return connected_;
}

RobotState UREpicsComm::get_robot_state() {
    namespace pvd = epics::pvData;

    if (not connected_ or not channel_) {
        return RobotState {
            .connected = false,
            .joint_angles = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
        };
    }

    std::vector<float> qvec(6);
    auto pfield = channel_->get();
    pvd::shared_vector<const double> q_shared = pfield->getSubFieldT<pvd::PVDoubleArray>("value")->view();
    std::copy(q_shared.begin(), q_shared.end(), qvec.begin());

    for (auto &v : qvec) {
        v = v * (M_PI/180.0);
    }

    return RobotState {
        .connected = true,
        .joint_angles = qvec
    };
}
