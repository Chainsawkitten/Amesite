#include "ParticleEmitter.hpp"
#include "../Particles/ParticleSystem.hpp"
#include "../Entity/Entity.hpp"
#include "Transform.hpp"

using namespace Component;

ParticleEmitter::ParticleEmitter(Entity* entity) : SuperComponent(entity) {
    //this->mMinEmitTime = minEmitTime;
    //this->mMaxEmitTime = maxEmitTime;
    //this->mRelative = relative;
    //mTimeToNext = minEmitTime + ((double)rand() / RAND_MAX) * (maxEmitTime - minEmitTime);
	lifetime = 0.0;
}

void ParticleEmitter::Update(double time, Entity* follow, ParticleSystem* particleSystem) {
    this->follow = follow;
    this->particleSystem = particleSystem;
    timeToNext -= time;
    while (timeToNext < 0.0) {
        timeToNext += minEmitTime + ((double)rand() / RAND_MAX) * (maxEmitTime - minEmitTime);
        EmitParticle();
    }
	lifetime += time;
}

void ParticleEmitter::EmitParticle() {
    if (emitterType == CUBOID) {
        glm::vec3 position;
        position.x = origin.x - size.x / 2.f + rand() / (RAND_MAX / size.x);
        position.y = origin.y - size.y / 2.f + rand() / (RAND_MAX / size.y);
        position.z = origin.z - size.z / 2.f + rand() / (RAND_MAX / size.z);
        EmitParticleAt(position);
    }
    else if (emitterType == POINT) {
        EmitParticleAt(origin);
    }
}

void ParticleEmitter::EmitParticleAt(glm::vec3 position) {
    if (relative)
        position += follow->GetComponent<Component::Transform>()->position;
    
    particleSystem->EmitParticle(position);
}
