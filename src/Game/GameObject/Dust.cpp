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
    emitter->maxEmitTime = 0.015;
    emitter->minEmitTime = 0.01;
    emitter->lifetime = 0.0;
    emitter->origin = glm::vec3(0.f, -5.f, 0.f);
    emitter->size = glm::vec3(40.f, 20.f, 40.f);
    emitter->relative = true;
    emitter->timeToNext = 0.0;
    emitter->particleType.minLifetime = .7f;
    emitter->particleType.maxLifetime = 1.5f;
    emitter->particleType.minVelocity = glm::vec3(-0.025f, -0.01f, -0.025f);
    emitter->particleType.maxVelocity = glm::vec3(0.025f, -0.1f, 0.025f);
    emitter->particleType.minSize = glm::vec2(0.025f, 0.025f);
    emitter->particleType.maxSize = glm::vec2(0.05f, 0.05f);
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(1.f, 0.5f, 0.5f);
}

Dust::~Dust() {
}
