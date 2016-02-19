#include "Explosion.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Component/Transform.hpp>
#include <Engine/Component/ParticleEmitter.hpp>
#include "../Component/LifeTime.hpp"

using namespace GameObject;

Explosion::Explosion(Scene* scene) : SuperGameObject(scene) {
    node = CreateEntity(scene);
    node->AddComponent<Component::Transform>()->Move(0.f, 2.f, 0.f);
    node->AddComponent<Component::LifeTime>();
    Component::ParticleEmitter* emitter = node->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02;
    emitter->minEmitTime = 0.016;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 0.0;
    emitter->particleType.textureIndex = Component::ParticleEmitter::FIRE;
    emitter->particleType.minLifetime = .01f * 20.f;
    emitter->particleType.maxLifetime = .02f * 20.f;
    emitter->particleType.minVelocity = glm::vec3(-1.f, -1.f, -1.f);
    emitter->particleType.maxVelocity = glm::vec3(1.f, 1.f, 1.f);
    emitter->particleType.minSize = glm::vec2(.5f, .5f);
    emitter->particleType.maxSize = glm::vec2(.7f, .7f);
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.8f, .8f, .8f);
    emitter->particleType.startAlpha = 0.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;
}

Explosion::~Explosion() {
}
