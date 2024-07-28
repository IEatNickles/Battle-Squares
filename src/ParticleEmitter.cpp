#include "ParticleEmitter.hpp"

#include <cstdlib>
#include <glad/glad.h>

Vec2 vertices[] = {
    Vec2(-0.5f, -0.5f),
    Vec2(-0.5f,  0.5f),
    Vec2( 0.5f,  0.5f),
    Vec2( 0.5f, -0.5f),
};

int elements[] = {
    0, 1, 2,
    0, 2, 3,
};

ParticleEmitter::ParticleEmitter() {
}

ParticleEmitter::ParticleEmitter(Vec2 position, float spawn_rate, float lifetime, int max_particles) {
    m_Position = position;
    m_SpawnRate = spawn_rate;
    m_Lifetime = lifetime;
    m_MaxParticles = max_particles;

    m_SSBO = Buffer(GL_SHADER_STORAGE_BUFFER, nullptr, sizeof(GPUParticle) * m_MaxParticles);
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, m_SSBO.getID(), 0, sizeof(GPUParticle) * m_MaxParticles);

    m_EBO = Buffer(GL_ELEMENT_ARRAY_BUFFER, elements, sizeof(elements));
    m_VBO = Buffer(GL_ARRAY_BUFFER, vertices, sizeof(vertices));
    glGenVertexArrays(1, &m_VAO);

    m_EBO.bind();
    m_VBO.bind();
    glBindVertexArray(m_VAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);

    m_Particles = std::vector<Particle>(m_MaxParticles);
    m_GPUParticles = std::vector<GPUParticle>(m_MaxParticles);
    m_Matrices = std::vector<Mat4>(m_MaxParticles, Mat4::identity());

    m_SpawnTime = m_SpawnRate;
    m_PoolIndex = 0;
}

void ParticleEmitter::update(Timestep ts) {
    if (m_Playing && m_SpawnTime <= 0.0f) {
        Particle& p = m_Particles.at(m_PoolIndex);
        p.life = m_Lifetime;
        p.transform = Transform(m_Position);
        p.transform.scale = Vec2(0.2f);
        p.velocity = Vec2::zero();
        p.velocity.x = ((rand() % 20) - 10) * 0.2f;

        m_SpawnTime = m_SpawnRate;

        m_PoolIndex = (m_PoolIndex + 1) % m_MaxParticles;
    }
    for (int i = 0; i < m_Particles.size(); i++) {
        Particle& p = m_Particles.at(i);
        if (p.life <= 0.0f) {
            continue;
        }
        p.life -= ts.delta;

        p.velocity.y -= 9.81f * ts.delta;
        p.transform.position = p.transform.position.add(p.velocity.mul(ts.delta));

        GPUParticle& gp = m_GPUParticles.at(i);
        gp.model_matrix = p.transform.getMatrix();
        gp.color = p.color;
        gp.life = p.life;
    }
    m_SpawnTime -= ts.delta;
}

void ParticleEmitter::render() {
    m_EBO.bind();
    m_VBO.bind();
    glBindVertexArray(m_VAO);

    m_SSBO.setSubData(&m_GPUParticles[0], 0, m_GPUParticles.size() * sizeof(GPUParticle));

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, m_MaxParticles);
}

void ParticleEmitter::setPosition(const Vec2& position) {
    m_Position = position;
}

void ParticleEmitter::play() {
    m_Playing = true;
}

void ParticleEmitter::stop() {
    m_Playing = false;
}
