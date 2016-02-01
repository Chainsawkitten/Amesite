#include "ParticleEmitter.hpp"
#include "ParticleSystem.hpp"
#include "../Component/Transform.hpp"

ParticleEmitter::ParticleEmitter(double minEmitTime, double maxEmitTime, bool relative) {
    this->minEmitTime = minEmitTime;
    this->maxEmitTime = maxEmitTime;
    this->relative = relative;
    timeToNext = minEmitTime + ((double)rand() / RAND_MAX) * (maxEmitTime - minEmitTime);
	lifetime = 0.0;
}

void ParticleEmitter::Update(double time, ParticleSystem* particleSystem, Entity* follow) {
    this->particleSystem = particleSystem;
    this->follow = follow;
    
    timeToNext -= time;
    while (timeToNext < 0.0) {
        timeToNext += minEmitTime + ((double)rand() / RAND_MAX) * (maxEmitTime - minEmitTime);
        EmitParticle();
    }
	lifetime += time;
}

void ParticleEmitter::EmitParticleAt(glm::vec3 position) {
    if (relative)
        position += follow->GetComponent<Component::Transform>()->position;
    
    particleSystem->EmitParticle(position);
}

double ParticleEmitter::Lifetime() {
	return lifetime;
}

void ParticleEmitter::ResetLifetime() {
	lifetime = 0.0;
}
