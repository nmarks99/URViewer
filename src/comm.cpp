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


inline const std::string JOINT_ANGLES_PV_NAME = "Receive:ActualJointPositions";

void EPICSConnMon::connectEvent(const pvac::ConnectEvent &event) { connected_ = event.connected; }

UREpicsComm::UREpicsComm() : connection_monitor_(std::make_unique<EPICSConnMon>()) {
    epics::pvAccess::ca::CAClientFactory::start();
    provider_ = std::make_unique<pvac::ClientProvider>("ca");
}

void UREpicsComm::disconnect() {
    channel_->removeConnectListener(connection_monitor_.get());
    provider_->disconnect();
    channel_ = nullptr;
    monitor_ = nullptr;
    connected_ = false;
    connection_monitor_->connected_ = false;
};

bool UREpicsComm::connected() {
    return this->connection_monitor_->connected_;
};

bool UREpicsComm::connect(const std::string &ioc_prefix) {

    if (connected_) {
        this->disconnect();
    }
    // ioc_prefix might contain trailing '\0' characters so we do this to fix
    const std::string pv_name = std::string(ioc_prefix.c_str()) + JOINT_ANGLES_PV_NAME;
    channel_ = std::make_unique<pvac::ClientChannel>(provider_.get()->connect(pv_name));
    channel_->addConnectListener(connection_monitor_.get());
    monitor_ = std::make_unique<pvac::MonitorSync>(channel_->monitor());
    return true; // this "connect" never fails, though PV might not be connected
}

RobotState UREpicsComm::get_robot_state() {
    namespace pvd = epics::pvData;

    connected_ = this->connected();
    if (not connected_ or not channel_ or not monitor_) {
        return RobotState {
            .connected = false,
        };
    }

    // MonitorSync::test() is non-blocking as opposed to ClientChannel::get()
    if (monitor_->test()) {
        switch (monitor_->event.event) {
            case pvac::MonitorEvent::Data:
                while (monitor_->poll()) {
                    auto pfield = monitor_->root.get();
                    pvd::shared_vector<const double> q_shared = pfield->getSubFieldT<pvd::PVDoubleArray>("value")->view();
                    std::copy(q_shared.begin(), q_shared.end(), last_angles_.begin());
                    for (auto &v : last_angles_) {
                        v = v * (M_PI/180.0); // convert degrees to radians
                    }
                }
                break;
            case pvac::MonitorEvent::Disconnect:
                connected_ = false;
                break;
            case pvac::MonitorEvent::Fail:
                connected_ = false;
                break;
            case pvac::MonitorEvent::Cancel:
                connected_ = false;
                break;
        }
    }
    return RobotState {
        .connected = true,
        .joint_angles = last_angles_
    };
}

