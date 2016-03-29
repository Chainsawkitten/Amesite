#include "Dust.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include <Component/Transform.hpp>
#include <Component/Physics.hpp>
#include <Component/ParticleEmitter.hpp>

using namespace GameObject;

Dust::Dust(Scene* scene) : SuperGameObject(scene) {
    body = CreateEntity();
    body->AddComponent<Component::RelativeTransform>();
    body->AddComponent<Component::ParticleEmitter>();

    // Particle emitter.
    Component::ParticleEmitter* emitter = body->GetComponent<Component::ParticleEmitter>();

    emitter->emitterType = Component::ParticleEmitter::CUBOID;
    emitter->maxEmitTime = 0.15 * 4.f;
    emitter->minEmitTime = 0.1 * 4.f;
    emitter->lifetime = 0.0;
    emitter->size = glm::vec3(40.f, 20.f, 40.f) * 2.f;
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
