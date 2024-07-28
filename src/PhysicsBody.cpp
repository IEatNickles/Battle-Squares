#include "PhysicsBody.hpp"

PhysicsBody::PhysicsBody(bool is_static, int layer) : m_Static(is_static), m_Layer(layer) {
}

PhysicsBody::PhysicsBody(const Vec2& position, const Vec2& size, bool is_static, int layer)
    : PhysicsBody(is_static, layer) {
    m_Transform.position = position;
    m_Size = size;
}

void PhysicsBody::addForce(const Vec2& force) {
    m_Velocity = m_Velocity.add(force);
}

void PhysicsBody::update(float dt) {
    m_Transform.position = m_Transform.position.add(m_Velocity.mul(dt));
}

void PhysicsBody::setVelocity(const Vec2& velocity) {
    m_Velocity = velocity;
}

Vec2 PhysicsBody::getVelocity() const {
    return m_Velocity;
}

void PhysicsBody::setPosition(const Vec2& position) {
    m_Transform.position = position;
}

Vec2 PhysicsBody::getPosition() const {
    return m_Transform.position;
}

Transform& PhysicsBody::getTransform() {
    return m_Transform;
}

AABB PhysicsBody::getBounds() const {
    return AABB::centerSize(m_Transform.position, m_Size);
}

bool PhysicsBody::isStatic() const {
    return m_Static;
}

Vec2 PhysicsBody::getSize() const {
    return m_Size;
}

int PhysicsBody::getLayer() const {
    return m_Layer;
}
