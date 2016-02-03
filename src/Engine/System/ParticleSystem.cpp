#include "ParticleSystem.hpp"

#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"
#include "../Component/ParticleEmitter.hpp"
#include "../Scene/Scene.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <vector>
#include "../Resources.hpp"

using namespace System;

ParticleSystem::ParticleSystem() { 
    this->mMaxParticleCount = 1000;
    mParticleCount = 0;
}

ParticleSystem::~ParticleSystem() {
}

unsigned int ParticleSystem::ParticleCount() const {
    return mParticleCount;
}

unsigned int ParticleSystem::MaxParticleCount() const {
    return mMaxParticleCount;
}

void ParticleSystem::EmitParticle(Scene& scene, glm::vec3 position, Component::ParticleEmitter* emitter) {
    if (mParticleCount < mMaxParticleCount) {
        Particle particle;
        
        particle.worldPos = position;
        particle.life = 0.f;
        particle.lifetime = emitter->particleType.minLifetime + rand() / (RAND_MAX / (emitter->particleType.maxLifetime - emitter->particleType.minLifetime));
        
        if (emitter->particleType.uniformScaling) {
            particle.size = emitter->particleType.minSize + (rand() / static_cast<float>(RAND_MAX)) * (emitter->particleType.maxSize - emitter->particleType.minSize);
        } else {
            particle.size.x = emitter->particleType.minSize.x + rand() / (RAND_MAX / (emitter->particleType.maxSize.x - emitter->particleType.minSize.x));
            particle.size.y = emitter->particleType.minSize.y + rand() / (RAND_MAX / (emitter->particleType.maxSize.y - emitter->particleType.minSize.y));
        }
        
        particle.velocity.x = emitter->particleType.minVelocity.x + rand() / (RAND_MAX / (emitter->particleType.maxVelocity.x - emitter->particleType.minVelocity.x));
        particle.velocity.y = emitter->particleType.minVelocity.y + rand() / (RAND_MAX / (emitter->particleType.maxVelocity.y - emitter->particleType.minVelocity.y));
        particle.velocity.z = emitter->particleType.minVelocity.z + rand() / (RAND_MAX / (emitter->particleType.maxVelocity.z - emitter->particleType.minVelocity.z));
        
        scene.mParticles.push_back(particle);
        
        mParticleCount++;
    }
}

void ParticleSystem::Update(Scene& scene, double time) {
    if (!this->mParticles.empty()) {
        for (std::vector<int>::size_type i = 0; i != mParticles.size(); i++) {
            mParticles[i].life += static_cast<float>(time);
            
            if (mParticles[i].life >= mParticles[i].lifetime) {
                mParticles.erase(mParticles.begin() + i);
                mParticleCount--;
                i--;
            }
        }
    }
    
    for (unsigned int i = 0; i < scene.Size<Component::ParticleEmitter>(); i++) {
        Component::ParticleEmitter* emitter = scene.Get<Component::ParticleEmitter>(i);
        emitter->timeToNext -= time;
        while (emitter->timeToNext < 0.0) {
            emitter->timeToNext += emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
            EmitParticle(scene ,emitter);
        }
    }
    
    if (mParticleCount > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, mParticleCount * sizeof(ParticleSystem::Particle), &this->mParticles[0]);
    }
}

void ParticleSystem::EmitParticle(Scene& scene, Component::ParticleEmitter* emitter) {
    glm::vec3 position;
    if (emitter->emitterType == Component::ParticleEmitter::CUBOID) {
        position.x = emitter->origin.x - emitter->size.x / 2.f + rand() / (RAND_MAX / emitter->size.x);
        position.y = emitter->origin.y - emitter->size.y / 2.f + rand() / (RAND_MAX / emitter->size.y);
        position.z = emitter->origin.z - emitter->size.z / 2.f + rand() / (RAND_MAX / emitter->size.z);
    }
    else if (emitter->emitterType == Component::ParticleEmitter::POINT) {
        position = emitter->origin;
    }
    if (emitter->relative) {
        position += emitter->follow->GetComponent<Component::Transform>()->position;
    }
    EmitParticle(scene, position, emitter);
}