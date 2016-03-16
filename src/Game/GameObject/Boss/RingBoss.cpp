#include "RingBoss.hpp"

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

#include "../Player/Player1.hpp"
#include "../Player/Player2.hpp"
#include "../Bullet.hpp"

#include "../../Util/Hub.hpp"
#include "../../Util/ControlSchemes.hpp"
#include "../../Util/GameEntityFactory.hpp"

using namespace GameObject;

RingBoss::RingBoss(Scene* scene) : SuperBoss(scene) {
    mMaxSpawnerDelay = 0.9f;

    node->AddComponent<Component::Transform>()->scale *= 0.6f;
    node->AddComponent<Component::Update>()->updateFunction = std::bind(&RingBoss::mUpdateFunction, this);

    body->GetComponent<Component::Transform>()->scale *= 0.8f;
    body->AddComponent<Component::Mesh>()->geometry = mBodyModel = Resources().CreateOBJModel("Resources/diamond_body.obj");
    body->AddComponent<Component::Material>();
    body->GetComponent<Component::Material>()->SetDiffuse("Resources/pylon_diff.png");
    body->GetComponent<Component::Material>()->SetSpecular("Resources/pylon_spec.png");
    body->GetComponent<Component::Material>()->SetGlow("Resources/pylon_glow.png");
    body->AddComponent<Component::Collider2DCircle>()->radius = 6.f;
    body->AddComponent<Component::Explode>()->size = 120.f;
    body->GetComponent<Component::Explode>()->particleTextureIndex = Component::ParticleEmitter::PURPLE;
    body->GetComponent<Component::Explode>()->lifeTime = 1.5f;
    body->GetComponent<Component::Explode>()->offset.y = 5.0f;
    body->GetComponent<Component::Explode>()->sound = true;
    body->AddComponent<Component::Health>()->faction = 1;
    body->GetComponent<Component::Health>()->removeOnLowHealth = false;
    body->GetComponent<Component::Health>()->health = 300.f;
    body->AddComponent<Component::Spawner>()->delay = mMaxSpawnerDelay;
    body->GetComponent<Component::Spawner>()->faction = 1.f;
    //body->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AlwaysShootClosestPlayer);

    CreateRing();
}

RingBoss::~RingBoss() {
    Resources().FreeOBJModel(mBodyModel);
    Resources().FreeOBJModel(mRingModel);
}

float RingBoss::GetHealth() {
    return body->GetComponent<Component::Health>()->health;
}

glm::vec3 RingBoss::GetPosition() {
    return node->GetComponent<Component::Transform>()->position;
}

void RingBoss::CreateRing() {
    ring.node = CreateEntity();
    Component::RelativeTransform* transform = ring.node->AddComponent<Component::RelativeTransform>();
    transform->parentEntity = node;
    transform->scale *= 1.8f;
    ring.node->AddComponent<Component::Mesh>()->geometry = mRingModel = Resources().CreateOBJModel("Resources/ring_body.obj");
    ring.node->AddComponent<Component::Material>();
    ring.node->AddComponent<Component::Controller>()->controlSchemes.push_back(ControlScheme::LookAtClosestPlayer);
    ring.node->AddComponent<Component::Physics>()->angularDragFactor = 0.f;

    ring.midFront = CreateEntity();
    CreateRingPart(ring.midFront, glm::vec3(0.f, 0.f, 10.f));

    ring.left = CreateEntity();
    CreateRingPart(ring.left, glm::vec3(8.f, 0.f, 2.f));

    ring.right = CreateEntity();
    CreateRingPart(ring.right, glm::vec3(-8.f, 0.f, 2.f));

    ring.leftFront = CreateEntity();
    CreateRingPart(ring.leftFront, glm::vec3(8.f, 0.f, 10.f));

    ring.rightFront = CreateEntity();
    CreateRingPart(ring.rightFront, glm::vec3(-8.f, 0.f, 10.f));

    ring.leftBack = CreateEntity();
    CreateRingPart(ring.leftBack, glm::vec3(7.f, 0.f, -5.f));

    ring.rightBack = CreateEntity();
    CreateRingPart(ring.rightBack, glm::vec3(-7.f, 0.f, -5.f));

}

void GameObject::RingBoss::FireBullets() {
    float minimumDistance = std::numeric_limits<float>().max();
    glm::vec3 targetPlayerPosition;
    Component::Transform* transform = body->GetComponent<Component::Transform>();
    Component::Spawner* spawner = body->GetComponent<Component::Spawner>();


    glm::vec3 transformWorldPosition = transform->GetWorldPosition();
    for (auto& player : HubInstance().mPlayers) {
        if (player->Active()) {
            float distanceToPlayer = glm::distance(player->GetPosition(), transformWorldPosition);
            if (distanceToPlayer < minimumDistance) {
                minimumDistance = distanceToPlayer;
                targetPlayerPosition = player->GetPosition();
            }
        }
    }
    glm::vec3 targetDirection = targetPlayerPosition - transformWorldPosition;
    glm::vec3 normTarget = glm::normalize(targetDirection);
    GameObject::Bullet* bullets[5];

    if (glm::length(targetDirection) > 0.001f) {
        // Front
        bullets[0] = GameEntityCreator().CreateEnemyBullet(transform->GetWorldPosition(), glm::normalize(glm::vec3(0.f, 0.f, targetDirection.z)), spawner->faction);
        bullets[1] = GameEntityCreator().CreateEnemyBullet(transform->GetWorldPosition(), glm::normalize(glm::vec3(targetDirection.x, 0.f, targetDirection.z)), spawner->faction);
        bullets[2] = GameEntityCreator().CreateEnemyBullet(transform->GetWorldPosition(), glm::normalize(glm::vec3(targetDirection.x, 0.f, 0.f)), spawner->faction);

        // Back
        bullets[3] = GameEntityCreator().CreateEnemyBullet(transform->GetWorldPosition(), glm::normalize(glm::vec3(0.f, 0.f, -targetDirection.z)), spawner->faction);
        bullets[4] = GameEntityCreator().CreateEnemyBullet(transform->GetWorldPosition(), glm::normalize(glm::vec3(-targetDirection.x, 0.f, 0.f)), spawner->faction);
    }
    float maxHealth = body->GetComponent<Component::Health>()->maxHealth;
    float health = body->GetComponent<Component::Health>()->health;
    float healthFraction = ((maxHealth - health) / maxHealth);
    float lifeTimeFraction = 3.f;
    float size = 5.f + healthFraction * 4.f;
    float speed = 10.f + healthFraction * 5.f;

    body->GetComponent<Component::Spawner>()->delay = mMaxSpawnerDelay - healthFraction * .6f;

    for (auto bullet : bullets) {
        bullet->node->KillComponent<Component::PointLight>();
        bullet->node->GetComponent<Component::Transform>()->scale *= size;
        bullet->node->GetComponent<Component::Physics>()->velocity *= speed;
        bullet->node->GetComponent<Component::Physics>()->maxVelocity = 1.5f * glm::length(bullet->node->GetComponent<Component::Physics>()->velocity);

        bullet->node->GetComponent<Component::ParticleEmitter>()->particleType.minSize = glm::vec2(1.f, 1.f) * size;
        bullet->node->GetComponent<Component::ParticleEmitter>()->particleType.maxSize = glm::vec2(1.f, 1.f) * size;
        bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.minSize = glm::vec2(1.f, 1.f) * size*0.8f;
        bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.maxSize = glm::vec2(1.f, 1.f) * size*0.8f;

        bullet->node->GetComponent<Component::ParticleEmitter>()->particleType.textureIndex = Component::ParticleEmitter::DUST;
        bullet->node->GetComponent<Component::ParticleEmitter>()->particleType.color = glm::vec3(0.9f, 0.2f, 0.2f);
        bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.textureIndex = Component::ParticleEmitter::SMOKE;
        bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.color = glm::vec3(0.6f, 0.2f, 0.2f);

        bullet->node->GetComponent<Component::ParticleEmitter>()->particleType.minLifetime /= lifeTimeFraction;
        bullet->node->GetComponent<Component::ParticleEmitter>()->particleType.maxLifetime /= lifeTimeFraction;
        bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.minLifetime /= lifeTimeFraction;
        bullet->tail->GetComponent<Component::ParticleEmitter>()->particleType.maxLifetime /= lifeTimeFraction;
        bullet->node->GetComponent<Component::Damage>()->damageAmount = 9000.0000001f;
    }
}

void RingBoss::CreateRingPart(Entity* entity, glm::vec3 position) {
    Component::RelativeTransform* transform = entity->AddComponent<Component::RelativeTransform>();
    transform->parentEntity = ring.node;
    transform->position = position;
    entity->AddComponent<Component::Collider2DCircle>()->radius = 5.f;
    entity->AddComponent<Component::Reflect>()->faction = 1.f;
    entity->AddComponent<Component::Damage>()->faction = 1.f;
    entity->GetComponent<Component::Damage>()->removeOnImpact = false;
}

void RingBoss::Activate() {
    SuperBoss::Activate();
}

void RingBoss::Deactivate() {
    SuperBoss::Deactivate();
}

void RingBoss::mUpdateFunction() {
    SuperBoss::mUpdateFunction();
    Component::Spawner* spawner = body->GetComponent<Component::Spawner>();

    if (spawner != nullptr) {
        if (spawner->timeSinceSpawn >= spawner->delay) {
            spawner->timeSinceSpawn = 0.0f;
            FireBullets();
        }
    }
}
