#include "math/Transform.hpp"

Transform Transform::identity() {
    return Transform {
        .position = Vec2::zero(),
        .rotation = 0.0f,
        .scale = Vec2::one(),
        .shear = Vec2::zero()
    };
}

Mat4 Transform::getMatrix() const {
    return Mat4::fromTranslation(position).mul(Mat4::fromRotation(rotation)).mul(Mat4::fromScale(scale)).mul(Mat4::fromShear(shear));
}
