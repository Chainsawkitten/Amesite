#include "Dust.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/ParticleEmitter.hpp>

using namespace GameObject;

Dust::Dust(Scene* scene) : SuperGameObject(scene) {
    body = CreateEntity(scene);
    body->AddComponent<Component::ParticleEmitter>();

    // Particle emitter.
    Component::ParticleEmitter* emitter = body->GetComponent<Component::ParticleEmitter>();

    emitter->emitterType = Component::ParticleEmitter::CUBOID;
    emitter->maxEmitTime = 0.15;
    emitter->minEmitTime = 0.1;
    emitter->lifetime = 0.0;
    emitter->origin = glm::vec3(0.f, -5.f, 0.f);
    emitter->size = glm::vec3(100.f, 20.f, 100.f);
    emitter->relative = true;
    emitter->timeToNext = 0.0;
    emitter->particleType.minLifetime = 5.f;
    emitter->particleType.maxLifetime = 7.f;
    emitter->particleType.minVelocity = glm::vec3(-0.025f, -0.01f, -0.025f);
    emitter->particleType.maxVelocity = glm::vec3(0.025f, -0.1f, 0.025f);
    emitter->particleType.minSize = glm::vec2(0.08f, 0.08f);
    emitter->particleType.maxSize = glm::vec2(0.15f, 0.15f);
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.5f, 0.5f, 0.5f);
    emitter->particleType.startAlpha = 0.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;
}

Dust::~Dust() {
}
