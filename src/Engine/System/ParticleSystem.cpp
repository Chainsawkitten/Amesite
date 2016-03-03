#include "ParticleSystem.hpp"

#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"
#include "../Component/ParticleEmitter.hpp"

#include "../Entity/Entity.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Texture/Texture.hpp"
#include "../Util/Log.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <vector>
#include "../Resources.hpp"

using namespace System;

ParticleSystem* ParticleSystem::mActiveInstance = nullptr;

ParticleSystem::ParticleSystem() {
    mMaxParticleCount = 10000;
}

ParticleSystem::~ParticleSystem() {
    delete mActiveInstance;
}

unsigned int ParticleSystem::MaxParticleCount() const {
    return mMaxParticleCount;
}

void ParticleSystem::Update(Scene& scene, double time) {
    for (unsigned int i = 0; i < scene.GetParticleCount(); ++i) {
        scene.GetParticles()[i].life += static_cast<float>(time);
        if (scene.GetParticles()[i].life >= scene.GetParticles()[i].lifetime) {
            scene.GetParticles()[i--] = scene.GetParticles()[scene.GetParticleCount() - 1];
            scene.SetParticleCount(scene.GetParticleCount() - 1);
        }
    }
    
    std::list<Component::ParticleEmitter*> particleEmitters = scene.GetAll<Component::ParticleEmitter>();
    for (Component::ParticleEmitter* emitter : particleEmitters) {
        if (emitter->IsKilled())
            continue;
        
        if (emitter->enabled) {
            emitter->timeToNext -= time;
            while (emitter->timeToNext < 0.0) {
                emitter->timeToNext += emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
                EmitParticle(scene, emitter);
            }
        }
    }
}

ParticleSystem& ParticleSystem::GetActiveInstance() {
    if (mActiveInstance != nullptr) {
        return *mActiveInstance;
    } else {
        mActiveInstance = new ParticleSystem();
        return *mActiveInstance;
    }
}

ParticleSystem& System::Particle() {
    return ParticleSystem::GetActiveInstance();
}

void ParticleSystem::EmitParticle(Scene& scene, Component::ParticleEmitter* emitter) {
    Component::Transform* transform = emitter->entity->GetComponent<Component::Transform>();
    if (transform != nullptr) {
        glm::vec3 position;
        if (emitter->emitterType == Component::ParticleEmitter::CUBOID) {
            position.x = transform->GetWorldPosition().x - emitter->size.x / 2.f + rand() / (RAND_MAX / emitter->size.x);
            position.y = transform->GetWorldPosition().y - emitter->size.y / 2.f + rand() / (RAND_MAX / emitter->size.y);
            position.z = transform->GetWorldPosition().z - emitter->size.z / 2.f + rand() / (RAND_MAX / emitter->size.z);
        }
        else
            position = transform->GetWorldPosition();
        EmitParticle(scene, position, emitter);
    }
}

void ParticleSystem::EmitParticle(Scene& scene, glm::vec3 position, Component::ParticleEmitter* emitter) {
    if (scene.GetParticleCount() < mMaxParticleCount) {
        Particle particle;
        
        particle.worldPos = position;
        particle.life = 0.f;
        particle.lifetime = emitter->particleType.minLifetime + rand() / (RAND_MAX / (emitter->particleType.maxLifetime - emitter->particleType.minLifetime));
        particle.textureIndex = (float)emitter->particleType.textureIndex;
        particle.alpha[0] = emitter->particleType.startAlpha;
        particle.alpha[1] = emitter->particleType.midAlpha;
        particle.alpha[2] = emitter->particleType.endAlpha;
        particle.color = emitter->particleType.color;
        
        if (emitter->particleType.uniformScaling) {
            particle.size = emitter->particleType.minSize + (rand() / static_cast<float>(RAND_MAX)) * (emitter->particleType.maxSize - emitter->particleType.minSize);
        } else {
            particle.size.x = emitter->particleType.minSize.x + rand() / (RAND_MAX / (emitter->particleType.maxSize.x - emitter->particleType.minSize.x));
            particle.size.y = emitter->particleType.minSize.y + rand() / (RAND_MAX / (emitter->particleType.maxSize.y - emitter->particleType.minSize.y));
        }
        
        particle.velocity.x = emitter->particleType.minVelocity.x + rand() / (RAND_MAX / (emitter->particleType.maxVelocity.x - emitter->particleType.minVelocity.x));
        particle.velocity.y = emitter->particleType.minVelocity.y + rand() / (RAND_MAX / (emitter->particleType.maxVelocity.y - emitter->particleType.minVelocity.y));
        particle.velocity.z = emitter->particleType.minVelocity.z + rand() / (RAND_MAX / (emitter->particleType.maxVelocity.z - emitter->particleType.minVelocity.z));
        
        scene.GetParticles()[scene.GetParticleCount()] = particle;
        scene.SetParticleCount(scene.GetParticleCount() + 1);
    }
}
