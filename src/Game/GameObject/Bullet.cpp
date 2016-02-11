#include "Bullet.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/Cube.hpp>
#include "../Component/Damage.hpp"
#include "../Component/LifeTime.hpp"
#include <Engine/Component/ParticleEmitter.hpp>
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Collider2DCircle.hpp>

using namespace GameObject;

Bullet::Bullet(Scene* scene) : SuperGameObject(scene) {
    Entity* entity = mScene->CreateEntity();
    mEntityMap["body"] = entity;
    entity->AddComponent<Component::Damage>()->damageAmount = 10.f;
    entity->AddComponent<Component::Transform>()->scale = glm::vec3(0.5f, 0.5f, 0.5f);

    entity->AddComponent<Component::Collider2DCircle>()->radius = 0.25f;
    entity->AddComponent<Component::Physics>();
    entity->AddComponent<Component::LifeTime>()->lifeTime = 2.f;

    Entity* emitterEntity = mScene->CreateEntity();
    mEntityMap["emitter"] = emitterEntity;
    emitterEntity->AddComponent<Component::LifeTime>()->lifeTime = 1.8f;
    emitterEntity->AddComponent<Component::ParticleEmitter>();

    // Particle emitter.

    Component::ParticleEmitter* emitter = emitterEntity->GetComponent<Component::ParticleEmitter>();

    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02;
    emitter->minEmitTime = 0.016;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 0.0;
    emitter->relative = true;
    emitter->follow = entity;
    emitter->origin = glm::vec3(0.f, 0.f, 0.f);

    //Particle type.
    emitter->particleType.textureIndex = Component::ParticleEmitter::FIRE;
    emitter->particleType.minLifetime = .01f;
    emitter->particleType.maxLifetime = .02f;
    emitter->particleType.minVelocity = glm::vec3(-.3f, 0.f, -.2f);
    emitter->particleType.maxVelocity = glm::vec3(.3f, 0.f, .2f);
    emitter->particleType.minSize = glm::vec2(.4f, .4f);
    emitter->particleType.maxSize = glm::vec2(.7f, .7f);
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(1.f, 1.f, 1.f);
    emitter->particleType.startAlpha = 1.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;

    Entity* emitterEntityOne = mScene->CreateEntity();
    mEntityMap["emitter"] = emitterEntity;
    emitterEntityOne->AddComponent<Component::LifeTime>()->lifeTime = 1.8f;
    emitterEntityOne->AddComponent<Component::ParticleEmitter>();

    // Particle emitter.

    Component::ParticleEmitter* emitterOne = emitterEntityOne->GetComponent<Component::ParticleEmitter>();

    emitterOne->emitterType = Component::ParticleEmitter::POINT;
    emitterOne->maxEmitTime = 0.02;
    emitterOne->minEmitTime = 0.016;
    emitterOne->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitterOne->lifetime = 0.0;
    emitterOne->relative = true;
    emitterOne->follow = entity;
    emitterOne->origin = glm::vec3(0.f, 0.f, 0.f);

    //Particle type.
    emitterOne->particleType.textureIndex = Component::ParticleEmitter::BLUE;
    emitterOne->particleType.minLifetime = .013f;
    emitterOne->particleType.maxLifetime = .023f;
    emitterOne->particleType.minVelocity = glm::vec3(-.3f, 0.f, -.2f);
    emitterOne->particleType.maxVelocity = glm::vec3(.3f, 0.f, .2f);
    emitterOne->particleType.minSize = glm::vec2(.5f, .5f);
    emitterOne->particleType.maxSize = glm::vec2(.6f, .6f);
    emitterOne->particleType.uniformScaling = true;
    emitterOne->particleType.color = glm::vec3(.6f, .6f, .6f);
    emitterOne->particleType.startAlpha = .5f;
    emitterOne->particleType.midAlpha = 1.f;
    emitterOne->particleType.endAlpha = 0.f;

    mEntityVector.push_back(emitterEntity);
    mEntityVector.push_back(emitterEntityOne);
    mEntityVector.push_back(entity);
}

Bullet::~Bullet() {
    Resources().FreeCube();
}
