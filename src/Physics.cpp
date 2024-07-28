#include "Physics.hpp"
#include "PhysicsBody.hpp"

#include <cmath>

void Physics::step(Timestep ts) {
    for (PhysicsBody *body : m_Bodies) {
        if (!body->isStatic()) {
            body->addForce(Vec2(0.0f, GRAVITY * ts.delta));
        }
    }

    for (auto ita = m_Bodies.begin(); ita < m_Bodies.end() - 1; ita++) {
        for (auto itb = m_Bodies.begin() + 1; itb < m_Bodies.end(); itb++) {
            PhysicsBody *body_a = *ita;
            PhysicsBody *body_b = *itb;
            if (body_a->isStatic() && body_b->isStatic()) {
                continue;
            }
            if (!body_a->isStatic() && !body_b->isStatic()) {
                continue;
            }

            if (!body_a->isStatic() && body_b->isStatic()) {
                resolve(body_a, body_b, ts.delta);
            } else if (body_a->isStatic() && !body_b->isStatic()) {
                resolve(body_b, body_a, ts.delta);
            }
        }
    }

    for (PhysicsBody *body : m_Bodies) {
        if (!body->isStatic()) {
            body->update(ts.delta);
        }
    }
}

void Physics::resolve(PhysicsBody *a, PhysicsBody *b, float dt) {
    AABB bounds = b->getBounds().inflate(a->getSize());
    if (a->getBounds().intersects(bounds)) {
        RaycastHit hit;
        if (raycastAABB(a->getPosition(), a->getVelocity().mul(dt), bounds, hit)) {
            a->setVelocity(a->getVelocity().add(hit.normal.mul(a->getVelocity().abs()).mul(1.0f - hit.distance)));
        }
    }
}

bool Physics::raycast(Vec2 origin, Vec2 dir, RaycastHit& hit, int exclude_layers) const {
    for (PhysicsBody *body : m_Bodies) {
        if ((body->getLayer() & exclude_layers) > 0) {
            continue;
        }
        if (raycastBody(origin, dir, body, hit)) {
            return true;
        }
    }
    return false;
}

bool Physics::raycastBody(Vec2 origin, Vec2 dir, PhysicsBody *body, RaycastHit& hit) const {
    if (raycastAABB(origin, dir, body->getBounds(), hit)) {
        hit.body = body;
        return true;
    }
    return false;
}

bool Physics::raycastAABB(Vec2 origin, Vec2 dir, const AABB& bounds, RaycastHit& hit) const {
    Vec2 inv_dir = dir.reciporical();

    Vec2 t0 = bounds.min.sub(origin).mul(inv_dir);
    Vec2 t1 = bounds.max.sub(origin).mul(inv_dir);

    if (std::isnan(t0.x) || std::isnan(t0.y)) {
        return false;
    }
    if (std::isnan(t1.x) || std::isnan(t1.y)) {
        return false;
    }

    if (t0.x > t1.x) {
        std::swap(t0.x, t1.x);
    }
    if (t0.y > t1.y) {
        std::swap(t0.y, t1.y);
    }

    if (t0.x > t1.y || t0.y > t1.x) {
        return false;
    }

    float near = std::max(t0.x, t0.y);
    float far = std::min(t1.x, t1.y);

    if (far < 0.0f) {
        return false;
    }

    if (near < 0.0f || near >= 1.0f) {
        return false;
    }

    hit.distance = near;
    hit.point = origin.add(dir.mul(near));

    if (t0.x > t0.y) {
        if (inv_dir.x < 0.0f) {
            hit.normal = Vec2(1.0f, 0.0f);
        } else {
            hit.normal = Vec2(-1.0f, 0.0f);
        }
    } else if (t0.x < t0.y) {
        if (inv_dir.y < 0.0f) {
            hit.normal = Vec2(0.0f, 1.0f);
        } else {
            hit.normal = Vec2(0.0f, -1.0f);
        }
    }

    return true;
}

void Physics::addBody(PhysicsBody *body) {
    m_Bodies.push_back(body);
}
