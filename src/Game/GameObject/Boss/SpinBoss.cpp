#include "SpinBoss.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include <Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../../Component/Health.hpp"
#include "../../Component/Explode.hpp"
#include "../../Component/LifeTime.hpp"
#include "../../Component/Reflect.hpp"
#include "../../Component/Controller.hpp"
#include "../../Component/Spawner.hpp"
#include "../../Component/Update.hpp"
#include "../../Component/Damage.hpp"

#include <Component/Transform.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/Mesh.hpp>
#include <Component/Material.hpp>
#include <Component/Collider2DCircle.hpp>
#include <Component/Animation.hpp>
#include <Component/ParticleEmitter.hpp>
#include <Component/Physics.hpp>

#include "../Bullet.hpp"

#include "../../Util/ControlSchemes.hpp"
#include "../../Util/GameEntityFactory.hpp"

using namespace GameObject;

SpinBoss::SpinBoss(Scene* scene) : SuperBoss(scene) {
    mBulletSize = 5.f;
    node->AddComponent<Component::Transform>()->scale *= 1.f;

    node->AddComponent<Component::Update>()->updateFunction = std::bind(&SpinBoss::mUpdateFunction, this);
    node->AddComponent<Component::Physics>()->angularVelocity.y = 0.1f;
    node->GetComponent<Component::Physics>()->angularDragFactor = 0.f;

    mNoReflectTexture = Resources().CreateTexture2DFromFile("Resources/pylon_glow.png");
    mReflectTexture = Resources().CreateTexture2DFromFile("Resources/pylon_spec.png");

    body->AddComponent<Component::Damage>()->faction = 1;
    body->GetComponent<Component::Damage>()->removeOnImpact = false;
    body->AddComponent<Component::Mesh>()->geometry = mBody = Resources().CreateOBJModel("Resources/pylon.obj");
    body->AddComponent<Component::Material>();
    Resources().FreeTexture2D(body->GetComponent<Component::Material>()->glow);
    body->GetComponent<Component::Material>()->SetDiffuse("Resources/pylon_diff.png");
    body->GetComponent<Component::Material>()->SetSpecular("Resources/pylon_spec.png");
    body->GetComponent<Component::Material>()->glow = mReflectTexture;
    body->AddComponent<Component::Reflect>();
    body->AddComponent<Component::Collider2DCircle>()->radius = 6.f;
    body->AddComponent<Component::Explode>()->size = 60.f;
    body->GetComponent<Component::Explode>()->particleTextureIndex = Component::ParticleEmitter::PURPLE;
    body->GetComponent<Component::Explode>()->lifeTime = 1.5f;
    body->GetComponent<Component::Explode>()->offset.y = 5.0f;
    body->GetComponent<Component::Explode>()->sound = true;
    body->AddComponent<Component::Health>()->faction = 1;
    body->GetComponent<Component::Health>()->removeOnLowHealth = false;


    mState = BossState::REFLECT;
    mPhase = BossPhase::ONE;
    mNrOfArms = 6;

    for (int i = 0; i < mNrOfArms; i++) {
        armArr[i] = scene->CreateEntity();
        Component::RelativeTransform* transform = armArr[i]->AddComponent<Component::RelativeTransform>();
        transform->parentEntity = node;
        if (i < 3) {
            transform->yaw = 120.f * i;
            transform->position = transform->GetWorldDirection() * 15.f;
            transform->scale *= 0.5f;
            armArr[i]->AddComponent<Component::Physics>()->angularDragFactor = 0.f;
            armArr[i]->GetComponent<Component::Physics>()->angularVelocity.y = 0.25f;
        } else {
            transform->yaw = 120.f * i - 300.f;
            transform->position = transform->GetWorldDirection() * 17.f;
            transform->scale *= 0.3f;
            armArr[i]->AddComponent<Component::Physics>()->angularDragFactor = 0.f;
            armArr[i]->GetComponent<Component::Physics>()->angularVelocity.y = -0.25f;
        }
        CreateArm(armArr[i]);
    }
}

SpinBoss::~SpinBoss() {
    Resources().FreeOBJModel(mBody);
    if (mState != BossState::REFLECT)
        Resources().FreeTexture2D(mReflectTexture);
    if (mState != BossState::NOREFLECT)
        Resources().FreeTexture2D(mNoReflectTexture);
}

void SpinBoss::CreateArm(Entity* entity) {
    mCurrentSpawnerDelay = 0.35f;
    entity->AddComponent<Component::Damage>()->faction = 1;
    entity->GetComponent<Component::Damage>()->removeOnImpact = false;
    entity->AddComponent<Component::Collider2DCircle>()->radius = 6.f;
    entity->AddComponent<Component::Health>()->removeOnLowHealth = false;
    entity->GetComponent<Component::Health>()->faction = 1;
    entity->AddComponent<Component::Mesh>()->geometry = mBody;
    entity->AddComponent<Component::Material>();
    entity->GetComponent<Component::Material>()->SetDiffuse("Resources/pylon_diff.png");
    entity->GetComponent<Component::Material>()->SetSpecular("Resources/pylon_spec.png");
    entity->GetComponent<Component::Material>()->SetGlow("Resources/pylon_glow.png");
    entity->AddComponent<Component::Spawner>()->faction = 1;
    entity->GetComponent<Component::Spawner>()->delay = mCurrentSpawnerDelay;
    entity->AddComponent<Component::Explode>()->size = 30.f;
    entity->GetComponent<Component::Explode>()->particleTextureIndex = Component::ParticleEmitter::FIRE;
    entity->GetComponent<Component::Explode>()->lifeTime = 0.5f;
    entity->GetComponent<Component::Explode>()->offset.y = 5.0f;
    entity->GetComponent<Component::Explode>()->sound = true;
    Component::ParticleEmitter* emitter = entity->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.02;
    emitter->minEmitTime = 0.016;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 0.0;
    emitter->particleType.textureIndex = Component::ParticleEmitter::PURPLE;
    emitter->particleType.minLifetime = .01f * 20.f;
    emitter->particleType.maxLifetime = .02f * 20.f;
    emitter->particleType.minVelocity = glm::vec3(-.3f, 0.f, -.2f);
    emitter->particleType.maxVelocity = glm::vec3(.3f, 0.f, .2f);
    emitter->particleType.minSize = glm::vec2(.5f, .5f) * 8.f;
    emitter->particleType.maxSize = glm::vec2(.7f, .7f) * 8.f;
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(.8f, .8f, .8f);
    emitter->particleType.startAlpha = 0.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;
}

void SpinBoss::ChangePhase(BossPhase phase) {
    mPhase = phase;
    if (mPhase == BossPhase::ONE) {
        mState = BossState::REFLECT;
        body->GetComponent<Component::Material>()->glow = mReflectTexture;
        body->AddComponent<Component::Reflect>();
    } else {
        mState = BossState::NOREFLECT;
        body->GetComponent<Component::Material>()->glow = mNoReflectTexture;
        body->KillComponent<Component::Reflect>();
    }
}

void SpinBoss::FireBullet(glm::vec3 bossPos, glm::vec3 armPos) {
    GameObject::Bullet* bullet = GameEntityCreator().CreateEnemyBullet(armPos, armPos-bossPos, 1);
    float speed = 1.f;
    float lifeTimeFraction = 3.f;

    bullet->node->KillComponent<Component::PointLight>();
    bullet->node->GetComponent<Component::Transform>()->scale *= mBulletSize;
    bullet->node->GetComponent<Component::Physics>()->velocity = speed * (armPos - bossPos);
    bullet->node->GetComponent<Component::Physics>()->maxVelocity = 1.5f * glm::length(bullet->node->GetComponent<Component::Physics>()->velocity);
    bullet->node->GetComponent<Component::ParticleEmitter>()->particleType.minSize = glm::vec2(1.f, 1.f) * mBulletSize;
    bullet->node->GetComponent<Component::ParticleEmitter>()->particleType.maxSize = glm::vec2(1.f, 1.f) * mBulletSize;
    bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.minSize = glm::vec2(1.f, 1.f) * mBulletSize*0.8f;
    bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.maxSize = glm::vec2(1.f, 1.f) * mBulletSize*0.8f;

    bullet->node->GetComponent<Component::ParticleEmitter>()->particleType.textureIndex = Component::ParticleEmitter::PURPLE;
    bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.textureIndex = Component::ParticleEmitter::SMOKE;
    bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.color = glm::vec3(0.2f, 0.2f, 0.2f);

    bullet->node->GetComponent<Component::ParticleEmitter>()->particleType.minLifetime /= lifeTimeFraction;
    bullet->node->GetComponent<Component::ParticleEmitter>()->particleType.maxLifetime /= lifeTimeFraction;
    bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.minLifetime /= lifeTimeFraction;
    bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.maxLifetime /= lifeTimeFraction;

    bullet->node->GetComponent<Component::Damage>()->damageAmount = 100.f;
}

float SpinBoss::GetHealth() {
    return body->GetComponent<Component::Health>()->health;
}

glm::vec3 SpinBoss::GetPosition() {
    return node->GetComponent<Component::Transform>()->position;
}

void SpinBoss::Activate() {
    SuperBoss::Activate();
}

void SpinBoss::Deactivate() {
    SuperBoss::Deactivate();
}

void SpinBoss::mUpdateFunction() {
    SuperBoss::mUpdateFunction();
    glm::vec3 bossPosition = node->GetComponent<Component::Transform>()->position;

    if (mPhase == BossPhase::ONE)
        for (int i = 0; i < 6; i++)
            if (armArr[i] != nullptr) {
                if (armArr[i]->GetComponent<Component::Health>()->health < 0.01f) {
                    node->GetComponent<Component::Physics>()->angularVelocity.y += 0.15f;
                    armArr[i]->Kill();
                    armArr[i] = nullptr;
                    mCurrentSpawnerDelay -= 0.05;
                    mBulletSize += 1.f;
                    if (--mNrOfArms <= 0)
                        ChangePhase(BossPhase::TWO);
                } else {
                    Component::Spawner* spawnerComp = armArr[i]->GetComponent<Component::Spawner>();
                    spawnerComp->delay = mCurrentSpawnerDelay;
                    if (spawnerComp->timeSinceSpawn > spawnerComp->delay && Active()) {
                        FireBullet(bossPosition, armArr[i]->GetComponent<Component::RelativeTransform>()->GetWorldPosition());
                        spawnerComp->timeSinceSpawn = 0.f;
                    }
                }
            }
}
