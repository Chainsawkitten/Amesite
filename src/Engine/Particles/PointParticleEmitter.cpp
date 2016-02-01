#include "PointParticleEmitter.hpp"

PointParticleEmitter::PointParticleEmitter(glm::vec3 origin, double minEmitTime, double maxEmitTime, bool relative) : ParticleEmitter(minEmitTime, maxEmitTime, relative) {
    this->origin = origin;
}

void PointParticleEmitter::EmitParticle() {
    EmitParticleAt(origin);
}
