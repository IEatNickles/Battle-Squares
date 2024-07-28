#include "math/Vec4.hpp"
#include <cstdio>
#include <cstdlib>

#include "math/Mat4.hpp"

#define ASSERT(expr, fmt) if (!(expr)) {\
    printf("Assertion Failed: " fmt "\n");\
    exit(EXIT_FAILURE);\
}

float Vec4::get(int idx) const {
    ASSERT(idx < 4, "Cannot index component higher than 3");
    switch (idx) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
    }
    return 0;
}

Vec4 Vec4::add(const Vec4& other) {
    return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
}
Vec4 Vec4::sub(const Vec4& other) {
    return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
}
Vec4 Vec4::mul(const Vec4& other) {
    return Vec4(x * other.x, y * other.y, z * other.z, w * other.w);
}
Vec4 Vec4::div(const Vec4& other) {
    return Vec4(x / other.x, y / other.y, z / other.z, w / other.w);
}

Vec4 Vec4::mul(const Mat4& other) {
    return Vec4(
            other.rows[0].x * x + other.rows[0].y * x + other.rows[0].z * x + other.rows[0].w * x,
            other.rows[1].x * y + other.rows[1].y * y + other.rows[1].y * y + other.rows[1].w * y,
            other.rows[2].x * z + other.rows[2].z * y + other.rows[2].z * z + other.rows[2].w * z,
            other.rows[3].x * w + other.rows[3].w * y + other.rows[3].w * w + other.rows[3].w * w
            );
}

Vec4 Vec4::mul(float scalar) {
    return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
}
Vec4 Vec4::div(float scalar) {
    return Vec4(x / scalar, y / scalar, z / scalar, w / scalar);
}

float Vec4::dot(const Vec4& other) {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}
Vec4 Vec4::cross(const Vec4& other) {
    ASSERT(false, "Cross product is unimplemented");
    return Vec4::zero();
}

Vec4 Vec4::zero() {
    return Vec4(0.0f);
}

Vec4 Vec4::one() {
    return Vec4(1.0f);
}

void Vec4::print() const {
    printf("<%f, %f, %f, %f>\n", x, y, z, w);
}
