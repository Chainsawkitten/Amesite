#include "ShieldBoss.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include <Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../../Component/Health.hpp"
#include "../../Component/Explode.hpp"
#include "../../Component/Update.hpp"
#include "../../Component/Spawner.hpp"
#include "../../Component/Reflect.hpp"
#include "../../Component/Controller.hpp"
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

ShieldBoss::ShieldBoss(Scene* scene) : SuperBoss(scene) {

    mOffsetAngle = 0;

    node->AddComponent<Component::Transform>()->scale *= 0.7f;
    node->AddComponent<Component::Update>()->updateFunction = std::bind(&ShieldBoss::mUpdateFunction, this);
    node->AddComponent<Component::Spawner>()->delay = 2.f;
    node->AddComponent<Component::Physics>()->angularDragFactor = 0.f;
    node->GetComponent<Component::Physics>()->angularVelocity.y = 0.15f;

    body->GetComponent<Component::Transform>()->scale *= 1.2f;
    body->AddComponent<Component::Mesh>()->geometry = mBodyModel = Resources().CreateOBJModel("Resources/diamond_body.obj");
    body->AddComponent<Component::Material>();
    body->GetComponent<Component::Material>()->SetDiffuse("Resources/pylon_diff.png");
    body->GetComponent<Component::Material>()->SetSpecular("Resources/pylon_spec.png");
    body->GetComponent<Component::Material>()->SetGlow("Resources/pylon_glow.png");
    body->AddComponent<Component::Collider2DCircle>()->radius = 6.f;
    body->AddComponent<Component::Explode>()->size = 30.f;
    body->GetComponent<Component::Explode>()->particleTextureIndex = Component::ParticleEmitter::PURPLE;
    body->GetComponent<Component::Explode>()->lifeTime = 1.5f;
    body->GetComponent<Component::Explode>()->offset.y = 5.0f;
    body->GetComponent<Component::Explode>()->sound = true;
    body->AddComponent<Component::Health>()->removeOnLowHealth = false;
    body->GetComponent<Component::Health>()->faction = 1;
    body->GetComponent<Component::Health>()->maxCooldown = 10.f;
    body->GetComponent<Component::Health>()->health = body->GetComponent<Component::Health>()->maxHealth = 100.f;
    body->GetComponent<Component::Health>()->regainAmount = body->GetComponent<Component::Health>()->health / 3.f;
    body->AddComponent<Component::Physics>()->angularDragFactor = 0.f;
    body->GetComponent<Component::Physics>()->angularVelocity.y = -0.1f;

    mCoverModel = Resources().CreateOBJModel("Resources/ring_segment.obj");
    for (int i = 0; i < 3; i++) {
        coverArr[i].node = CreateEntity();
        coverArr[i].leftShield = CreateEntity();
        coverArr[i].rightShield = CreateEntity();
        CreateCover(&coverArr[i]);
        Component::Transform* transform = coverArr[i].node->GetComponent<Component::Transform>();
        transform->yaw = i * 120.f;
        transform->Move(transform->GetWorldDirection() * 21.f);
    }
}

ShieldBoss::~ShieldBoss() {
    Resources().FreeOBJModel(mBodyModel);
    Resources().FreeOBJModel(mCoverModel);
}

float ShieldBoss::GetHealth() {
    return body->GetComponent<Component::Health>()->health;
}

glm::vec3 ShieldBoss::GetPosition() {
    return node->GetComponent<Component::Transform>()->position;
}

void ShieldBoss::CreateCover(Cover* cover) {
    cover->node->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    cover->node->GetComponent<Component::Transform>()->scale *= 2.f;
    cover->node->AddComponent<Component::Mesh>()->geometry = mCoverModel;
    cover->node->AddComponent<Component::Material>();

    cover->node->AddComponent<Component::Reflect>();
    cover->node->AddComponent<Component::Collider2DCircle>()->radius = 4.f;
    cover->node->AddComponent<Component::Damage>()->faction = 1.f;
    cover->node->GetComponent<Component::Damage>()->removeOnImpact = false;

    cover->leftShield->AddComponent<Component::RelativeTransform>()->parentEntity = cover->node;
    cover->leftShield->GetComponent<Component::Transform>()->position.x = 5.f;
    cover->leftShield->AddComponent<Component::Reflect>();
    cover->leftShield->AddComponent<Component::Collider2DCircle>()->radius = 4.f;
    cover->leftShield->AddComponent<Component::Damage>()->faction = 1.f;
    cover->leftShield->GetComponent<Component::Damage>()->removeOnImpact = false;

    cover->rightShield->AddComponent<Component::RelativeTransform>()->parentEntity = cover->node;
    cover->rightShield->GetComponent<Component::Transform>()->position.x = -5.f;
    cover->rightShield->AddComponent<Component::Reflect>();
    cover->rightShield->AddComponent<Component::Collider2DCircle>()->radius = 4.f;
    cover->rightShield->AddComponent<Component::Damage>()->faction = 1.f;
    cover->rightShield->GetComponent<Component::Damage>()->removeOnImpact = false;

    Deactivate();
}

void ShieldBoss::Activate() {
    SuperBoss::Activate();
}

void ShieldBoss::Deactivate() {
    SuperBoss::Deactivate();
}

void ShieldBoss::mUpdateFunction() {
    SuperBoss::mUpdateFunction();
    Component::Spawner* spawnerComp = node->GetComponent<Component::Spawner>();
    if (spawnerComp->timeSinceSpawn > spawnerComp->delay && Active()) {
        FireBullets();
        spawnerComp->timeSinceSpawn = 0.f;
    }
}

void ShieldBoss::FireBullets() {
    int nr = 15;
    mOffsetAngle += 360.f / nr / 2.f;
    mOffsetAngle = mOffsetAngle % 360;
    for (int i = 0; i < nr; i++) {
        float angle = mOffsetAngle + i * 360.f / nr;
        float size = 2.f;
        float speed = 20.f;
        GameObject::Bullet* bullet = GameEntityCreator().CreateEnemyBullet(GetPosition(), glm::vec3(0.f, 0.f, 0.f), 1);
        bullet->node->GetComponent<Component::Transform>()->scale *= size;
        bullet->node->GetComponent<Component::Transform>()->yaw = angle;
        bullet->node->GetComponent<Component::Physics>()->velocity = speed * bullet->node->GetComponent<Component::Transform>()->GetWorldDirection();
        bullet->node->GetComponent<Component::Physics>()->maxVelocity = 1.5f * glm::length(bullet->node->GetComponent<Component::Physics>()->velocity);
        bullet->node->GetComponent<Component::ParticleEmitter>()->particleType.minSize *= size;
        bullet->node->GetComponent<Component::ParticleEmitter>()->particleType.maxSize *= size;
        bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.minSize *= size;
        bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.maxSize *= size;
        bullet->node->GetComponent<Component::ParticleEmitter>()->particleType.minLifetime /= size;
        bullet->node->GetComponent<Component::ParticleEmitter>()->particleType.maxLifetime /= size;
        bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.minLifetime /= size;
        bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.maxLifetime /= size;
        bullet->node->GetComponent<Component::Damage>()->damageAmount = 9000.0000001f;
    }
}
