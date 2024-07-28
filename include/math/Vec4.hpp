#pragma once

struct Mat4;

struct Vec4 {
    float x, y, z, w;
    inline Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    inline Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    inline Vec4(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}

    float get(int idx) const;

    Vec4 add(const Vec4& other);
    Vec4 sub(const Vec4& other);
    Vec4 mul(const Vec4& other);
    Vec4 div(const Vec4& other);

    Vec4 mul(float scalar);
    Vec4 mul(const Mat4& other);
    Vec4 div(float scalar);

    float dot(const Vec4& other);
    Vec4 cross(const Vec4& other);

    static Vec4 zero();
    static Vec4 one();

    void print() const;
};
