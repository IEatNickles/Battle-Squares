#include "math/AABB.hpp"

AABB AABB::centerSize(Vec2 center, Vec2 size) {
    Vec2 half_extents = size.mul(0.5f);
    return AABB(center.sub(half_extents), center.add(half_extents));
}

bool AABB::intersects(const AABB& other) const {
    return other.min.x < max.x && other.max.x > min.x && other.min.y < max.y && other.max.y > min.y;
}

bool AABB::contains(const Vec2& point) const {
    return point.x < min.x && point.x > max.x && point.y < min.y && point.y > max.y;
}

AABB AABB::inflate(const Vec2& scale) const {
    return AABB::centerSize(center(), size().add(scale));
}
AABB AABB::inflate(float scale_x, float scale_y) const {
    return AABB::centerSize(center(), size().add(Vec2(scale_x, scale_y)));
}
AABB AABB::inflate(float scale) const {
    return AABB::centerSize(center(), size().add(Vec2(scale)));
}

Vec2 AABB::size() const {
    return Vec2(max.x - min.x, max.y - min.y);
}
Vec2 AABB::center() const {
    return size().mul(0.5f).add(min);
}
