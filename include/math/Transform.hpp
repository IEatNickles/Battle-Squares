#pragma once

#include "math/Mat4.hpp"
#include "math/Vec2.hpp"

struct Transform {
    Vec2 position;
    float rotation;
    Vec2 scale;
    Vec2 shear;

    static Transform identity();

    Mat4 getMatrix() const;
};
