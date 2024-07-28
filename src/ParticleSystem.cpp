#include "ParticleSystem.hpp"

void ParticleSystem::update(Timestep ts) {
    for (ParticleEmitter *e : m_Emitters) {
        e->update(ts);
    }
}

void ParticleSystem::render() {
    for (ParticleEmitter *e : m_Emitters) {
        e->render();
    }
}

void ParticleSystem::addEmitter(ParticleEmitter *emitter) {
    m_Emitters.push_back(emitter);
}
