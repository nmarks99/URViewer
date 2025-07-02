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
    base_(model_dir_ / "base.obj", UR_MODEL_LABELS.at(0).data()),
    shoulder_(model_dir_ / "shoulder.obj", UR_MODEL_LABELS.at(1).data()),
    upperarm_(model_dir_ / "upperarm.obj", UR_MODEL_LABELS.at(2).data()),
    forearm_(model_dir_ / "forearm.obj", UR_MODEL_LABELS.at(3).data()),
    wrist1_(model_dir_ / "wrist1.obj", UR_MODEL_LABELS.at(4).data()),
    wrist2_(model_dir_ / "wrist2.obj", UR_MODEL_LABELS.at(5).data()),
    wrist3_(model_dir_ / "wrist3.obj", UR_MODEL_LABELS.at(6).data())
{

    switch (version) {
    case URVersion::UR3e:
        base_.model.transform = UR3e::TSBASE;
        shoulder_.model.transform = MatrixMultiply(UR3e::TB1, base_.model.transform);
        upperarm_.model.transform = MatrixMultiply(UR3e::T12, shoulder_.model.transform);
        forearm_.model.transform = MatrixMultiply(UR3e::T23, upperarm_.model.transform);
        wrist1_.model.transform = MatrixMultiply(UR3e::T34, forearm_.model.transform);
        wrist2_.model.transform = MatrixMultiply(UR3e::T45, wrist1_.model.transform);
        wrist3_.model.transform = MatrixMultiply(UR3e::T56, wrist2_.model.transform);
        break;
    case URVersion::UR5e:
        break;
    }
}

void UR::update(const std::vector<float> &joint_angles) {
    shoulder_.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(0)), UR3e::TB1), base_.model.transform);
    upperarm_.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(1)), UR3e::T12), shoulder_.model.transform);
    forearm_.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(2)), UR3e::T23), upperarm_.model.transform);
    wrist1_.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(3)), UR3e::T34), forearm_.model.transform);
    wrist2_.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(4)), UR3e::T45), wrist1_.model.transform);
    wrist3_.model.transform =
        MatrixMultiply(MatrixMultiply(MatrixRotateZ(joint_angles.at(5)), UR3e::T56), wrist2_.model.transform);
}

void UR::draw() {
    base_.draw();
    shoulder_.draw();
    upperarm_.draw();
    forearm_.draw();
    wrist1_.draw();
    wrist2_.draw();
    wrist3_.draw();
}

void UR::draw_axes() {
    shoulder_.draw_axes();
    upperarm_.draw_axes();
    forearm_.draw_axes();
    wrist1_.draw_axes();
    wrist2_.draw_axes();
    wrist3_.draw_axes();
}

// void UR::draw_axes(int mask) {
    // for (int i = 0; i < UR_NUM_AXES; i++) {
        // if (mask & (1 << i)) {
            // this->at(i).draw_axes();
        // }
    // }
// }
void UR::draw_axes(const std::array<bool, UR_NUM_AXES> &mask) {
    for (int i = 0; i < UR_NUM_AXES; i++) {
        if (mask.at(i)) {
            this->at(i).draw_axes();
        }
    }
}

RLModel& UR::at(int i) {
    switch (i) {
        case 0:
            return shoulder_;
        case 1:
            return upperarm_;
        case 2:
            return forearm_;
        case 3:
            return wrist1_;
        case 4:
            return wrist2_;
        case 5:
            return wrist3_;
        default:
            throw std::out_of_range("Index must be 0 to 5");
    }
}

