#include "Bullet.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Engine/Geometry/Geometry3D.hpp>
#include <Engine/Geometry/Cube.hpp>
#include "../Component/Damage.hpp"
#include "../Component/LifeTime.hpp"
#include "../Component/Explode.hpp"
#include "../Component/GridCollide.hpp"
#include <Engine/Component/ParticleEmitter.hpp>
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/PointLight.hpp>
#include "../Component/Update.hpp"


using namespace GameObject;

Bullet::Bullet(Scene* scene, float lifeTime) : SuperGameObject(scene) {

    lightIntensity = 1.f;

    node = CreateEntity();
    node->AddComponent<Component::Damage>()->damageAmount = 10.f;
    node->AddComponent<Component::Transform>()->scale = glm::vec3(0.5f, 0.5f, 0.5f);
    node->AddComponent<Component::Explode>()->lifeTime = 0.15f;
    node->GetComponent<Component::Explode>()->size = 2.f;
    node->GetComponent<Component::Explode>()->particleTextureIndex = Component::ParticleEmitter::FIRE;
    node->AddComponent<Component::Collider2DCircle>()->radius = 0.5f;
    node->AddComponent<Component::GridCollide>()->removeOnImpact = true;
    node->AddComponent<Component::Physics>();
    Component::LifeTime* lifeTimeComponent = node->AddComponent<Component::LifeTime>();
    lifeTimeComponent->lifeTime = lifeTimeComponent->initialLifeTime = lifeTime;

    node->AddComponent<Component::Update>()->updateFunction = std::bind(&Bullet::mUpdateFunction, this);

    Component::ParticleEmitter* emitter = node->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02;
    emitter->minEmitTime = 0.016;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 0.0;
    emitter->particleType.textureIndex = Component::ParticleEmitter::FIRE;
    emitter->particleType.minLifetime = .01f * 4.f;
    emitter->particleType.maxLifetime = .02f * 4.f;
    emitter->particleType.minVelocity = glm::vec3(-.3f, 0.f, -.2f);
    emitter->particleType.maxVelocity = glm::vec3(.3f, 0.f, .2f);
    emitter->particleType.minSize = glm::vec2(.5f, .5f) * 1.5f;
    emitter->particleType.maxSize = glm::vec2(.7f, .7f) * 1.5f;
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.8f, .8f, .8f);
    emitter->particleType.startAlpha = 1.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;

    auto light = node->AddComponent<Component::PointLight>();
    light->attenuation = 1.f;

    // Second emitter - blue
    tail = CreateEntity();
    tail->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    emitter = tail->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02;
    emitter->minEmitTime = 0.016;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 0.0;
    emitter->particleType.textureIndex = Component::ParticleEmitter::BLUE;
    emitter->particleType.minLifetime = .026f * 5.f;
    emitter->particleType.maxLifetime = .029f * 5.f;
    emitter->particleType.minVelocity = glm::vec3(-.3f, 0.f, -.2f) / 2.f;
    emitter->particleType.maxVelocity = glm::vec3(.3f, 0.f, .2f) / 2.f;
    emitter->particleType.minSize = glm::vec2(.5f, .5f);
    emitter->particleType.maxSize = glm::vec2(.6f, .6f);
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.6f, .6f, .6f);
    emitter->particleType.startAlpha = .5f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;
}

Bullet::~Bullet() {
}

void Bullet::mUpdateFunction() {
    Component::LifeTime* lifeTimeComponent = node->GetComponent<Component::LifeTime>();
    float lifeTimeFactor = 1.f - lifeTimeComponent->lifeTime / lifeTimeComponent->initialLifeTime;
    float maxIntensity = 15.f;
    float timeLimFactor = 0.1f / lifeTimeComponent->initialLifeTime;
    Component::PointLight* light = node->GetComponent<Component::PointLight>();
    if (light != nullptr) {
        if (lifeTimeFactor < timeLimFactor) {
            light->intensity = maxIntensity / timeLimFactor * lifeTimeFactor * lightIntensity;
        } else {
            light->intensity = maxIntensity / (timeLimFactor - 1.f) * (lifeTimeFactor - 1.f) * lightIntensity;
        }
    }
       
}
