#include "Explosion.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Component/Transform.hpp>
#include <Engine/Component/ParticleEmitter.hpp>
#include "../Component/LifeTime.hpp"

using namespace GameObject;

Explosion::Explosion(Scene* scene) : SuperGameObject(scene) {
    node = CreateEntity();
    node->AddComponent<Component::Transform>()->Move(0.f, 2.f, 0.f);
    node->AddComponent<Component::LifeTime>();

    // Fire emitter
    Component::ParticleEmitter* emitter = node->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02;
    emitter->minEmitTime = 0.016;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 0.0;
    emitter->particleType.textureIndex = Component::ParticleEmitter::FIRE;
    emitter->particleType.minLifetime = .01f * 20.f;
    emitter->particleType.maxLifetime = .02f * 20.f;
    emitter->particleType.minVelocity = glm::vec3(-10.f, 10.f, -10.f);
    emitter->particleType.maxVelocity = glm::vec3(10.f, -10.f, 10.f);
    emitter->particleType.minSize = glm::vec2(.1f, .1f);
    emitter->particleType.maxSize = glm::vec2(.5f, .5f);
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.8f, .8f, .8f);
    emitter->particleType.startAlpha = 0.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;

    // Smoke emitter
    tail = CreateEntity();
    tail->AddComponent<Component::Transform>()->Move(0.f, 2.f, 0.f);
    tail->AddComponent<Component::LifeTime>();

    Component::ParticleEmitter* smokeEmitter = tail->AddComponent<Component::ParticleEmitter>();
    smokeEmitter->emitterType = Component::ParticleEmitter::POINT;
    smokeEmitter->maxEmitTime = 0.02;
    smokeEmitter->minEmitTime = 0.016;
    smokeEmitter->timeToNext = smokeEmitter->minEmitTime + ((double)rand() / RAND_MAX) * (smokeEmitter->maxEmitTime - smokeEmitter->minEmitTime);
    smokeEmitter->lifetime = 0.0;
    smokeEmitter->particleType.textureIndex = Component::ParticleEmitter::STAR;
    smokeEmitter->particleType.minLifetime = .01f * 20.f;
    smokeEmitter->particleType.maxLifetime = .02f * 20.f;
    smokeEmitter->particleType.minVelocity = glm::vec3(-25.f, 25.f, -25.f);
    smokeEmitter->particleType.maxVelocity = glm::vec3(25.f, -25.f, 25.f);
    smokeEmitter->particleType.minSize = glm::vec2(.08f, .08f)*1.f;
    smokeEmitter->particleType.maxSize = glm::vec2(.19f, .19f)*1.f;
    smokeEmitter->particleType.uniformScaling = true;
    smokeEmitter->particleType.color = glm::vec3(.1f, .1f, .1f);
    smokeEmitter->particleType.startAlpha = 0.f;
    smokeEmitter->particleType.midAlpha = 1.f;
    smokeEmitter->particleType.endAlpha = 0.f;

    // Blue emitter
    body = CreateEntity();
    body->AddComponent<Component::Transform>()->Move(0.f, 2.f, 0.f);
    body->AddComponent<Component::LifeTime>();

    Component::ParticleEmitter* blueEmitter = body->AddComponent<Component::ParticleEmitter>();
    blueEmitter->emitterType = Component::ParticleEmitter::POINT;
    blueEmitter->maxEmitTime = 0.02;
    blueEmitter->minEmitTime = 0.016;
    blueEmitter->timeToNext = blueEmitter->minEmitTime + ((double)rand() / RAND_MAX) * (blueEmitter->maxEmitTime - blueEmitter->minEmitTime);
    blueEmitter->lifetime = 0.0;
    blueEmitter->particleType.textureIndex = Component::ParticleEmitter::SMOKE_SECOND;
    blueEmitter->particleType.minLifetime = .01f * 20.f;
    blueEmitter->particleType.maxLifetime = .02f * 20.f;
    blueEmitter->particleType.minVelocity = glm::vec3(-5.f, 5.f, -5.f);
    blueEmitter->particleType.maxVelocity = glm::vec3(5.f, -5.f, 5.f);
    blueEmitter->particleType.minSize = glm::vec2(.08f, .08f)*3.f;
    blueEmitter->particleType.maxSize = glm::vec2(.19f, .19f)*3.f;
    blueEmitter->particleType.uniformScaling = true;
    blueEmitter->particleType.color = glm::vec3(.2f, .2f, .2f);
    blueEmitter->particleType.startAlpha = 0.f;
    blueEmitter->particleType.midAlpha = 1.f;
    blueEmitter->particleType.endAlpha = 0.f;
}

Explosion::~Explosion() {
}
