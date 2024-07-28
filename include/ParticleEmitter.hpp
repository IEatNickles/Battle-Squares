#pragma once

#include "Timestep.hpp"
#include "math/Mat4.hpp"
#include "math/Transform.hpp"
#include "math/Vec2.hpp"
#include <cstdint>
#include <vector>

#include "Buffer.hpp"

struct GPUParticle {
    Mat4 model_matrix = Mat4::identity();
    Vec4 color = Vec4::one();
    float life = 0.0f;
    float a = 0.0f, b = 0.0f, c = 0.0f;
};

struct Particle {
    Transform transform = Transform::identity();
    Vec4 color = Vec4::one();
    Vec2 velocity = Vec2::zero();
    float life = 0.0f;
};

class ParticleEmitter {
    public:
        ParticleEmitter();
        ParticleEmitter(Vec2 position, float spawn_rate, float lifetime, int max_particles);

        void update(Timestep ts);
        void render();

        void play();
        void stop();

        void setPosition(const Vec2& position);

    private:
        Vec2 m_Position;
        float m_SpawnRate;
        float m_Lifetime;
        int m_MaxParticles;

        float m_SpawnTime;

        std::vector<Particle> m_Particles;
        std::vector<GPUParticle> m_GPUParticles;
        int m_PoolIndex;

        bool m_Playing;

        std::vector<Mat4> m_Matrices;

        Buffer m_SSBO;
        Buffer m_VBO;
        Buffer m_EBO;
        uint32_t m_VAO;
};
