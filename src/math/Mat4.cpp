#include "math/Mat4.hpp"
#include <cmath>
#include <cstdio>

#include "math/Vec2.hpp"

Mat4 Mat4::fromRows(Vec4 r0, Vec4 r1, Vec4 r2, Vec4 r3) {
    return Mat4 { r0, r1, r2, r3 };
}

Mat4 Mat4::fromCols(Vec4 c0, Vec4 c1, Vec4 c2, Vec4 c3) {
    return Mat4 {
            Vec4(c0.x, c1.x, c2.x, c3.x),
            Vec4(c0.y, c1.y, c2.y, c3.y),
            Vec4(c0.z, c1.z, c2.z, c3.z),
            Vec4(c0.w, c1.w, c2.w, c3.w) };
}

Mat4 Mat4::fromTranslation(const Vec2& translation) {
    return Mat4::fromRows(
            Vec4(1.0f, 0.0f, 0.0f, 0.0f),
            Vec4(0.0f, 1.0f, 0.0f, 0.0f),
            Vec4(0.0f, 0.0f, 1.0f, 0.0f),
            Vec4(translation.x, translation.y, 0.0f, 1.0f));
}

Mat4 Mat4::fromRotation(float rotation) {
    float c = cos(rotation);
    float s = sin(rotation);
    return Mat4::fromRows(
            Vec4(c,    s, 0.0f, 0.0f),
            Vec4(-s,   c, 0.0f, 0.0f),
            Vec4(0.0f, 0.0f, 1.0f, 0.0f),
            Vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

Mat4 Mat4::fromScale(const Vec2& scale) {
    return Mat4::fromRows(
            Vec4(scale.x, 0.0f, 0.0f, 0.0f),
            Vec4(0.0f, scale.y, 0.0f, 0.0f),
            Vec4(0.0f, 0.0f, 1.0f, 0.0f),
            Vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

Mat4 Mat4::add(const Mat4& other) {
    return Mat4::fromRows(
            rows[0].add(other.rows[0]),
            rows[1].add(other.rows[1]),
            rows[2].add(other.rows[2]),
            rows[3].add(other.rows[3])
    );
}
Mat4 Mat4::sub(const Mat4& other) {
    return Mat4::fromRows(
            rows[0].sub(other.rows[0]),
            rows[1].sub(other.rows[1]),
            rows[2].sub(other.rows[2]),
            rows[3].sub(other.rows[3])
    );
}
Mat4 Mat4::mul(const Mat4& other) {
    return Mat4::fromCols(
            Vec4(getCol(0).dot(other.rows[0]), getCol(0).dot(other.rows[1]), getCol(0).dot(other.rows[2]), getCol(0).dot(other.rows[3])),
            Vec4(getCol(1).dot(other.rows[0]), getCol(1).dot(other.rows[1]), getCol(1).dot(other.rows[2]), getCol(1).dot(other.rows[3])),
            Vec4(getCol(2).dot(other.rows[0]), getCol(2).dot(other.rows[1]), getCol(2).dot(other.rows[2]), getCol(2).dot(other.rows[3])),
            Vec4(getCol(3).dot(other.rows[0]), getCol(3).dot(other.rows[1]), getCol(3).dot(other.rows[2]), getCol(3).dot(other.rows[3]))
    );
}

Mat4 Mat4::transpose() {
    return Mat4::fromCols(rows[0], rows[1], rows[2], rows[3]);
}

Vec4 Mat4::getCol(int idx) const {
    return Vec4(rows[0].get(idx), rows[1].get(idx), rows[2].get(idx), rows[3].get(idx));
}

Mat4 Mat4::createOrtho(float l, float r, float b, float t, float n, float f) {
    return Mat4::fromRows(
            Vec4(2.0f / (r - l), 0.0f,           0.0f,            -((r + l) / (r - l))),
            Vec4(0.0f,           2.0f / (t - b), 0.0f,            -((t + b) / (t - b))),
            Vec4(0.0f,           0.0f,           -2.0f / (f - n), -((f + n) / (f - n))),
            Vec4(0.0f,           0.0f,           0.0f,            1.0f)
    );
}

void Mat4::printRows() const {
    printf("[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n\n",
            rows[0].x, rows[0].y, rows[0].z, rows[0].w,
            rows[1].x, rows[1].y, rows[1].z, rows[1].w,
            rows[2].x, rows[2].y, rows[2].z, rows[2].w,
            rows[3].x, rows[3].y, rows[3].z, rows[3].w
          );
}

void Mat4::printCols() const {
    printf("[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n\n",
            getCol(0).x, getCol(0).y, getCol(0).z, getCol(0).w,
            getCol(1).x, getCol(1).y, getCol(1).z, getCol(1).w,
            getCol(2).x, getCol(2).y, getCol(2).z, getCol(2).w,
            getCol(3).x, getCol(3).y, getCol(3).z, getCol(3).w
          );
}
