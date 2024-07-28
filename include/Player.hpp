#pragma once

#include "ParticleEmitter.hpp"
#include "Physics.hpp"
#include "Sound.hpp"
#include "Timestep.hpp"
#include "math/Mat4.hpp"
#include "math/Vec2.hpp"

class ParticleSystem;
class PhysicsBody;
class Input;
struct Vec2;

class Player {
    public:
        Player(ParticleSystem& ps);
        ~Player();

        void update(Timestep ts, const Input& input, const Physics& physics);

        // TEMP
        void setPos(const Vec2& pos);
        Mat4 getMatrix() const;

        void foo(PhysicsBody*, Vec2);

        PhysicsBody* getBody() const;

    private:
        float m_Speed;
        PhysicsBody *m_Body;

        int m_MaxJumps;
        int m_CurrentJumps;
        int m_LastJumpTime;
        float m_JumpHeight;
        Sound m_JumpSound;

        int m_LastStepTime;
        Sound m_MoveSound;

        float m_MoveDir;
        bool m_Grounded;

        Vec2 m_Squish;
        float m_Shear;

        Sound m_CollideSound;

        ParticleEmitter *m_WalkParticles;
};
