#include "ParticleEmitter.hpp"
#include "ParticleSystem.hpp"
#include "../Component/Transform.hpp"

ParticleEmitter::ParticleEmitter(double minEmitTime, double maxEmitTime, bool relative) {
    this->mMinEmitTime = minEmitTime;
    this->mMaxEmitTime = maxEmitTime;
    this->mRelative = relative;
    mTimeToNext = minEmitTime + ((double)rand() / RAND_MAX) * (maxEmitTime - minEmitTime);
	mLifetime = 0.0;
}

void ParticleEmitter::Update(double time, ParticleSystem* particleSystem, Entity* follow) {
    this->mParticleSystem = particleSystem;
    this->mFollow = follow;
    
    mTimeToNext -= time;
    while (mTimeToNext < 0.0) {
        mTimeToNext += mMinEmitTime + ((double)rand() / RAND_MAX) * (mMaxEmitTime - mMinEmitTime);
        EmitParticle();
    }
	mLifetime += time;
}

void ParticleEmitter::EmitParticleAt(glm::vec3 position) {
    if (mRelative)
        position += mFollow->GetComponent<Component::Transform>()->position;
    
    mParticleSystem->EmitParticle(position);
}

double ParticleEmitter::Lifetime() {
	return mLifetime;
}

void ParticleEmitter::ResetLifetime() {
	mLifetime = 0.0;
}
