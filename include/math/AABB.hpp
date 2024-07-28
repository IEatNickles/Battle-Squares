#pragma once

#include "math/Vec2.hpp"

struct AABB {
    Vec2 min, max;

    AABB(Vec2 min, Vec2 max) : min(min), max(max) {}
    AABB(float min_x, float min_y, float max_x, float max_y) : min(min_x, min_y), max(max_x, max_y) {}

    static AABB centerSize(Vec2 center, Vec2 size);

    bool intersects(const AABB& other) const;
    bool contains(const Vec2& point) const;

    AABB inflate(const Vec2& scale) const;
    AABB inflate(float scale_x, float scale_y) const;
    AABB inflate(float scale) const;

    Vec2 size() const;
    Vec2 center() const;
};
