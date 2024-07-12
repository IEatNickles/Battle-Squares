#include "math/Vec2.hpp"
#include <cmath>
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

Vec2 Vec2::add(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
}
Vec2 Vec2::sub(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
}
Vec2 Vec2::mul(const Vec2& other) const {
    return Vec2(x * other.x, y * other.y);
}
Vec2 Vec2::div(const Vec2& other) const {
    return Vec2(x / other.x, y / other.y);
}

Vec2 Vec2::mul(float scalar) const {
    return Vec2(x * scalar, y * scalar);
}
Vec2 Vec2::div(float scalar) const {
    return Vec2(x / scalar, y / scalar);
}

Vec2 Vec2::reciporical() const {
    return Vec2(1.0f / x, 1.0f / y);
}

Vec2 Vec2::abs() const {
    return Vec2(std::abs(x), std::abs(y));
}

Vec2 Vec2::normalized() const {
    return this->div(length());
}

Vec2 Vec2::transpose() const {
    return Vec2(y, x);
}

float Vec2::length() const {
    return sqrt(lengthSq());
}
float Vec2::lengthSq() const {
    return x * x + y * y;
}

float Vec2::dot(const Vec2& other) const {
    return x * other.x + y * other.y;
}
