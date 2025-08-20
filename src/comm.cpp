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

            auto t0 = std::chrono::high_resolution_clock::now();
            qvec_double = recv_->getActualQ();
            auto t1 = std::chrono::high_resolution_clock::now();

            double elapsed_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
            std::cout << "[RTDE] recv_->getActualQ() took " << elapsed_ms << " ms\n";

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

UREpicsComm::UREpicsComm() {
    epics::pvAccess::ca::CAClientFactory::start();
    provider_ = std::make_unique<pvac::ClientProvider>("ca");
}

void UREpicsComm::disconnect() {
    channel_ = nullptr;
    connected_ = false;
};

bool UREpicsComm::connected() {
    return connected_;
};

bool UREpicsComm::connect(const std::string &ioc_prefix) {
    if (not connected_) {
        // ioc_prefix might contain trailing '\0' characters so we do this to fix
        const std::string pv_name = std::string(ioc_prefix.c_str()) + JOINT_ANGLES_PV_NAME;
        try {
            channel_ = std::make_unique<pvac::ClientChannel>(provider_.get()->connect(pv_name));
            connected_ = true;
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
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

