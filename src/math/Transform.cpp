#include "math/Transform.hpp"

Transform Transform::identity() {
    return Transform { Vec2::zero(), 0.0f, Vec2::one() };
}

Mat4 Transform::getMatrix() const {
    return Mat4::fromTranslation(translation).mul(Mat4::fromRotation(rotation)).mul(Mat4::fromScale(scale));
}
