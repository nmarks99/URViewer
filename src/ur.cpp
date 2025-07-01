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
    shoulder(model_dir_ / "shoulder.obj"),
    upperarm(model_dir_ / "upperarm.obj"),
    forearm(model_dir_ / "forearm.obj"),
    wrist1(model_dir_ / "wrist1.obj"),
    wrist2(model_dir_ / "wrist2.obj"),
    wrist3(model_dir_ / "wrist3.obj")
{

    switch (version) {
    case URVersion::UR3e:
        base.model.transform = UR3e::TSBASE;
        shoulder.model.transform = MatrixMultiply(UR3e::TB1, base.model.transform);
        upperarm.model.transform = MatrixMultiply(UR3e::T12, shoulder.model.transform);
        forearm.model.transform = MatrixMultiply(UR3e::T23, upperarm.model.transform);
        wrist1.model.transform = MatrixMultiply(UR3e::T34, forearm.model.transform);
        wrist2.model.transform = MatrixMultiply(UR3e::T45, wrist1.model.transform);
        wrist3.model.transform = MatrixMultiply(UR3e::T56, wrist2.model.transform);
        break;
    case URVersion::UR5e:
        break;
    }
}

void UR::update(const std::vector<float> &joint_angles) {
    shoulder.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(0)), UR3e::TB1), base.model.transform);
    upperarm.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(1)), UR3e::T12), shoulder.model.transform);
    forearm.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(2)), UR3e::T23), upperarm.model.transform);
    wrist1.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(3)), UR3e::T34), forearm.model.transform);
    wrist2.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(4)), UR3e::T45), wrist1.model.transform);
    wrist3.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(5)), UR3e::T56), wrist2.model.transform);
}

void UR::draw() {
    base.draw();
    shoulder.draw();
    upperarm.draw();
    forearm.draw();
    wrist1.draw();
    wrist2.draw();
    wrist3.draw();
}

void UR::draw_axes() {
    base.draw_axes();
    shoulder.draw_axes();
    upperarm.draw_axes();
    forearm.draw_axes();
    wrist1.draw_axes();
    wrist2.draw_axes();
    wrist3.draw_axes();
}
