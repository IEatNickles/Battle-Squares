#pragma once

#include <vector>

#include "Timestep.hpp"
#include "math/Vec2.hpp"
#include "math/AABB.hpp"

constexpr int GRAVITY = -30;

class PhysicsBody;

struct RaycastHit {
    Vec2 point;
    Vec2 normal;
    float distance;
    PhysicsBody *body;
};

class Physics {
    public:
        void step(Timestep ts);
        bool raycast(Vec2 origin, Vec2 dir, RaycastHit& hit, int exclude_layers = 0) const;
        void addBody(PhysicsBody *body);

    private:
        void resolve(PhysicsBody *a, PhysicsBody *b, float dt);

        bool raycastBody(Vec2 origin, Vec2 dir, PhysicsBody *body, RaycastHit& hit) const;
        bool raycastAABB(Vec2 origin, Vec2 dir, const AABB& bounds, RaycastHit& hit) const;

    private:
        std::vector<PhysicsBody*> m_Bodies;
};
