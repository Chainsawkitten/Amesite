#include "CuboidParticleEmitter.hpp"

CuboidParticleEmitter::CuboidParticleEmitter(glm::vec3 origin, glm::vec3 size, double minEmitTime, double maxEmitTime, bool relative) : ParticleEmitter(minEmitTime, maxEmitTime, relative) {
    this->origin = origin;
    this->size = size;
}

void CuboidParticleEmitter::EmitParticle() {
    glm::vec3 position;
    position.x = origin.x - size.x / 2.f + rand() / (RAND_MAX / size.x);
    position.y = origin.y - size.y / 2.f + rand() / (RAND_MAX / size.y);
    position.z = origin.z - size.z / 2.f + rand() / (RAND_MAX / size.z);
    
    EmitParticleAt(position);
}
