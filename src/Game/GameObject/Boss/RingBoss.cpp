#include "RingBoss.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../../Component/Health.hpp"
#include "../../Component/Explode.hpp"
#include "../../Component/Update.hpp"
#include "../../Component/Spawner.hpp"
#include "../../Component/Reflect.hpp"
#include "../../Component/Controller.hpp"
#include "../../Component/Damage.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/Animation.hpp>
#include <Engine/Component/ParticleEmitter.hpp>
#include <Engine/Component/Physics.hpp>

#include "../../Util/ControlSchemes.hpp"

using namespace GameObject;

RingBoss::RingBoss(Scene* scene) : SuperBoss(scene) {
    node->AddComponent<Component::Transform>()->scale *= 0.6f;
    node->AddComponent<Component::Update>()->updateFunction = std::bind(&RingBoss::mUpdateFunction, this);

    body->GetComponent<Component::Transform>()->scale *= 0.8f;
    body->AddComponent<Component::Mesh>()->geometry = mBodyModel = Resources().CreateOBJModel("Resources/diamond_body.obj");
    body->AddComponent<Component::Material>();
    body->GetComponent<Component::Material>()->SetDiffuse("Resources/Crystal_01_diff.png");
    body->GetComponent<Component::Material>()->SetSpecular("Resources/Crystal_01_spec.png");
    body->GetComponent<Component::Material>()->SetGlow("Resources/Crystal_01_glow.png");
    body->AddComponent<Component::Collider2DCircle>()->radius = 6.f;
    body->AddComponent<Component::Explode>()->size = 100.f;
    body->GetComponent<Component::Explode>()->particleTextureIndex = Component::ParticleEmitter::PURPLE;
    body->GetComponent<Component::Explode>()->lifeTime = 1.5f;
    body->GetComponent<Component::Explode>()->offset.y = 5.0f;
    body->GetComponent<Component::Explode>()->sound = true;
    body->AddComponent<Component::Health>()->faction = 1;
    body->GetComponent<Component::Health>()->removeOnLowHealth = false;
    body->AddComponent<Component::Spawner>()->delay = 0.2;
    body->GetComponent<Component::Spawner>()->faction = 1.f;
    body->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AlwaysShootClosestPlayer);

    CreateRing(scene);
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

void RingBoss::CreateRing(Scene* scene) {
    ring.node = scene->CreateEntity();
    Component::RelativeTransform* transform = ring.node->AddComponent<Component::RelativeTransform>();
    transform->parentEntity = node;
    transform->scale *= 1.8f;
    ring.node->AddComponent<Component::Mesh>()->geometry = mRingModel = Resources().CreateOBJModel("Resources/ring_body.obj");
    ring.node->AddComponent<Component::Material>();
    ring.node->AddComponent<Component::Controller>()->controlSchemes.push_back(ControlScheme::LookAtClosestPlayer);

    ring.midFront = scene->CreateEntity();
    CreateRingPart(ring.midFront, glm::vec3(0.f, 0.f, 10.f));

    ring.left = scene->CreateEntity();
    CreateRingPart(ring.left, glm::vec3(8.f, 0.f, 2.f));

    ring.right = scene->CreateEntity();
    CreateRingPart(ring.right, glm::vec3(-8.f,0.f, 2.f));

    ring.leftFront = scene->CreateEntity();
    CreateRingPart(ring.leftFront, glm::vec3(8.f, 0.f, 10.f));

    ring.rightFront = scene->CreateEntity();
    CreateRingPart(ring.rightFront, glm::vec3(-8.f, 0.f, 10.f));

    ring.leftBack = scene->CreateEntity();
    CreateRingPart(ring.leftBack, glm::vec3(7.f, 0.f, -5.f));

    ring.rightBack = scene->CreateEntity();
    CreateRingPart(ring.rightBack, glm::vec3(-7.f, 0.f, -5.f));

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
}
