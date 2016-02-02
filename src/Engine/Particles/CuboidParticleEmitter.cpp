#include "CuboidParticleEmitter.hpp"

CuboidParticleEmitter::CuboidParticleEmitter(glm::vec3 origin, glm::vec3 size, double minEmitTime, double maxEmitTime, bool relative) : ParticleEmitter(minEmitTime, maxEmitTime, relative) {
    this->mOrigin = origin;
    this->mSize = size;
}

void CuboidParticleEmitter::EmitParticle() {
    glm::vec3 position;
    position.x = mOrigin.x - mSize.x / 2.f + rand() / (RAND_MAX / mSize.x);
    position.y = mOrigin.y - mSize.y / 2.f + rand() / (RAND_MAX / mSize.y);
    position.z = mOrigin.z - mSize.z / 2.f + rand() / (RAND_MAX / mSize.z);
    
    EmitParticleAt(position);
}
