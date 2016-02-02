#include "PointParticleEmitter.hpp"
#include "../Component/ParticleEmitter.hpp"

PointParticleEmitter::PointParticleEmitter(glm::vec3 origin, Entity* entity) : ParticleEmitter(entity) {
    this->mOrigin = origin;
}

void PointParticleEmitter::EmitParticle() {
    EmitParticleAt(mOrigin);
}
