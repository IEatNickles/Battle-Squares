#pragma once

struct Vec2 {
    float x, y;
    inline Vec2(float x, float y) : x(x), y(y) {}
    inline Vec2(float scalar) : x(scalar), y(scalar) {}

    static Vec2 zero();
    static Vec2 one();

    Vec2 add(const Vec2& other);
    Vec2 sub(const Vec2& other);
    Vec2 mul(const Vec2& other);
    Vec2 div(const Vec2& other);

    Vec2 mul(float scalar);
    Vec2 div(float scalar);

    float dot(const Vec2& other);
    Vec2 cross(const Vec2& other);
};

#define VEC2_ZERO Vec2(0.0f);
#define VEC2_ONE Vec2(1.0f);
