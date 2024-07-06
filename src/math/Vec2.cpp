#include "math/Vec2.hpp"
#include <cstdio>
#include <cstdlib>

#define ASSERT(expr, fmt) if (!(expr)) {\
    printf("Assertion Failed: " fmt);\
    exit(EXIT_FAILURE);\
}

Vec2 Vec2::zero() {
    return Vec2(0.0f);
}

Vec2 Vec2::one() {
    return Vec2(1.0f);
}

Vec2 Vec2::add(const Vec2& other) {
    return Vec2(x + other.x, y + other.y);
}
Vec2 Vec2::sub(const Vec2& other) {
    return Vec2(x - other.x, y - other.y);
}
Vec2 Vec2::mul(const Vec2& other) {
    return Vec2(x * other.x, y * other.y);
}
Vec2 Vec2::div(const Vec2& other) {
    return Vec2(x / other.x, y / other.y);
}

Vec2 Vec2::mul(float scalar) {
    return Vec2(x * scalar, y * scalar);
}
Vec2 Vec2::div(float scalar) {
    return Vec2(x / scalar, y / scalar);
}

float Vec2::dot(const Vec2& other) {
    return x * other.x + y * other.y;
}
Vec2 Vec2::cross(const Vec2& other) {
    ASSERT(false, "Cross product is unimplemented");
    return Vec2(0.0f);
}
