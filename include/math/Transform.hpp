#pragma once

#include "math/Mat4.hpp"
#include "math/Vec2.hpp"

struct Transform {
    Vec2 translation;
    float rotation;
    Vec2 scale;

    static Transform identity();

    Mat4 getMatrix() const;
};
