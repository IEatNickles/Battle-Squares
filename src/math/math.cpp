#include "math/math.hpp"

float math::lerp(float a, float b, float t) {
    return t * (b - a) + a;
}

float math::invLerp(float a, float b, float v) {
    return (v - a) / (b - a);
}

