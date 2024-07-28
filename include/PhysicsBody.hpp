#pragma once

#include "math/Transform.hpp"
#include "math/Vec2.hpp"
#include "math/AABB.hpp"

enum PhysicsLayer {
    PL_NONE =   0x0001,
    PL_PLAYER = 0x0010,
};

class PhysicsBody;
using CollideCallback = void(*)(PhysicsBody*, Vec2);

class PhysicsBody {
    public:
        PhysicsBody(bool is_static = false, int layer = PL_NONE);
        PhysicsBody(const Vec2& position, const Vec2& size, bool is_static = false, int layer = PL_NONE);

        void addForce(const Vec2& force);
        void update(float dt);

        void setVelocity(const Vec2& velocity);
        Vec2 getVelocity() const;

        void setPosition(const Vec2& position);
        Vec2 getPosition() const;

        Transform& getTransform();

        AABB getBounds() const;

        bool isStatic() const;
        Vec2 getSize() const;

        int getLayer() const;

    private:
        Transform m_Transform;
        Vec2 m_Velocity;

        Vec2 m_Size;
        bool m_Static;

        int m_Layer;
};
