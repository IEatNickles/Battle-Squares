#include "ParticleEmitter.hpp"
#include "Timestep.hpp"

#include <vector>

class ParticleSystem {
    public:
        void update(Timestep ts);
        void render();

        void addEmitter(ParticleEmitter *emitter);

    private:
        std::vector<ParticleEmitter*> m_Emitters;
};
