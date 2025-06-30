#include "ur.hpp"

std::string get_model_dir(URVersion version) {
    // cmake defines the URVIEWER_MODEL_DIR macro
    std::filesystem::path model_dir = URVIEWER_MODEL_DIR;

    switch (version) {
    case URVersion::UR3e:
        model_dir = model_dir / "UR3e";
        break;
    case URVersion::UR5e:
        model_dir = model_dir / "UR5e";
        break;
    }
    return model_dir.string();
}

UR::UR(URVersion version) :
    model_dir_(get_model_dir(version)),
    base(model_dir_ / "base.obj"),
    link1(model_dir_ / "shoulder.obj"),
    link2(model_dir_ / "upperarm.obj"),
    link3(model_dir_ / "forearm.obj"),
    link4(model_dir_ / "wrist1.obj"),
    link5(model_dir_ / "wrist2.obj"),
    link6(model_dir_ / "wrist3.obj")
{

    switch (version) {
    case URVersion::UR3e:
        base.model.transform = UR3e::TSBASE;
        link1.model.transform = MatrixMultiply(UR3e::TB1, base.model.transform);
        link2.model.transform = MatrixMultiply(UR3e::T12, link1.model.transform);
        link3.model.transform = MatrixMultiply(UR3e::T23, link2.model.transform);
        link4.model.transform = MatrixMultiply(UR3e::T34, link3.model.transform);
        link5.model.transform = MatrixMultiply(UR3e::T45, link4.model.transform);
        link6.model.transform = MatrixMultiply(UR3e::T56, link5.model.transform);
        break;
    case URVersion::UR5e:
        break;
    }
}

void UR::update(const std::vector<double> &joint_angles) {
    link1.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(0)), UR3e::TB1), base.model.transform);
    link2.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(1)), UR3e::T12), link1.model.transform);
    link3.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(2)), UR3e::T23), link2.model.transform);
    link4.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(3)), UR3e::T34), link3.model.transform);
    link5.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(4)), UR3e::T45), link4.model.transform);
    link6.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(5)), UR3e::T56), link5.model.transform);
}

void UR::draw() {
    base.draw_wires();
    link1.draw_wires();
    link2.draw_wires();
    link3.draw_wires();
    link4.draw_wires();
    link5.draw_wires();
    // link6.draw_wires();
}

void UR::draw_axes() {
    base.draw_axes();
    link1.draw_axes();
    link2.draw_axes();
    link3.draw_axes();
    link4.draw_axes();
    link5.draw_axes();
    // link6.draw_axes();
}
