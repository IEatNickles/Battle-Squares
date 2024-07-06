#pragma once

#include "math/Vec4.hpp"

struct Vec2;

struct Mat4 {
    Vec4 rows[4];

    static Mat4 fromRows(Vec4 c0, Vec4 c1, Vec4 c2, Vec4 c3);
    static Mat4 fromCols(Vec4 c0, Vec4 c1, Vec4 c2, Vec4 c3);

    static Mat4 fromTranslation(const Vec2& translation);
    static Mat4 fromRotation(float rotation);
    static Mat4 fromScale(const Vec2& scale);

    Mat4 add(const Mat4& other);
    Mat4 sub(const Mat4& other);
    Mat4 mul(const Mat4& other);

    Mat4 transpose();

    Vec4 getCol(int idx) const;

    static Mat4 createOrtho(float left, float right, float bottom, float top, float near, float far);

    void printRows() const;
    void printCols() const;
};
