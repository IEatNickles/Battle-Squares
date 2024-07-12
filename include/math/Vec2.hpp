#pragma once

struct Vec2 {
    float x, y;
    inline Vec2() : x(0.0f), y(0.0f) {}
    inline Vec2(float x, float y) : x(x), y(y) {}
    inline Vec2(float scalar) : x(scalar), y(scalar) {}

    static Vec2 zero();
    static Vec2 one();

    Vec2 add(const Vec2& other) const;
    Vec2 sub(const Vec2& other) const;
    Vec2 mul(const Vec2& other) const;
    Vec2 div(const Vec2& other) const;

    Vec2 mul(float scalar) const;
    Vec2 div(float scalar) const;

    Vec2 reciporical() const;

    Vec2 abs() const;

    Vec2 normalized() const;

    Vec2 transpose() const;

    float length() const;
    float lengthSq() const;

    float dot(const Vec2& other) const;
};
