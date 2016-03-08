#include "Rocket.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../../Component/Controller.hpp"
#include "../../Component/Health.hpp"
#include "../../Component/Spawner.hpp"
#include "../../Component/Explode.hpp"
#include "../../Component/Update.hpp"
#include "../../Component/GridCollide.hpp"
#include "../../Component/Damage.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/Animation.hpp>
#include <Engine/Component/ParticleEmitter.hpp>

#include "../../Util/ControlSchemes.hpp"

using namespace GameObject;

Rocket::Rocket(Scene* scene) : SuperEnemy(scene) {
    mActiveGlow = Resources().CreateTexture2DFromFile("Resources/Crystal_01_glow.png");

    node->AddComponent<Component::Transform>()->scale *= 0.15f;
    node->AddComponent<Component::Collider2DCircle>()->radius = 8.0f;
    node->AddComponent<Component::Physics>()->velocityDragFactor = 1.f;
    node->GetComponent<Component::Physics>()->maxAngularVelocity *= 2.f;
    node->AddComponent<Component::Health>()->faction = 1;
    node->GetComponent<Component::Health>()->health = 500.f;
    node->GetComponent<Component::Health>()->removeOnLowHealth = false;
    node->AddComponent<Component::Explode>()->lifeTime = 0.25f;
    node->GetComponent<Component::Explode>()->size = 8.f;
    node->GetComponent<Component::Explode>()->particleTextureIndex = Component::ParticleEmitter::PURPLE;
    node->GetComponent<Component::Explode>()->sound = true;
    node->AddComponent<Component::Update>()->updateFunction = std::bind(&Rocket::mUpdateFunction, this);
    node->AddComponent<Component::Controller>()->controlSchemes.push_back(ControlScheme::LookAtClosestPlayer);
    node->GetComponent<Component::Controller>()->controlSchemes.push_back(ControlScheme::AccelerateTowardsClosestPlayer);
    node->AddComponent<Component::GridCollide>()->removeOnImpact = false;
    node->AddComponent<Component::Damage>()->faction = 1;
    node->GetComponent<Component::Damage>()->removeOnImpact = false;

    body = CreateEntity();
    body->AddComponent<Component::RelativeTransform>()->Move(0, 0, 5.5f);
    body->GetComponent<Component::RelativeTransform>()->parentEntity = node;
    body->AddComponent<Component::Mesh>()->geometry = mBodyModel = Resources().CreateOBJModel("Resources/rocket_body.obj");
    body->AddComponent<Component::Material>();
    //body->GetComponent<Component::Material>()->SetDiffuse("Resources/enemy_head_crystal_diff.png");
    //body->GetComponent<Component::Material>()->SetSpecular("Resources/enemy_head_crystal_spec.png");
    mDeactiveGlow = body->GetComponent<Component::Material>()->glow;
    Component::Animation::AnimationClip* idleHead = body->AddComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleHead->CreateKeyFrame(glm::vec3(-0.15f, 0.f, 0.f), 0.f, 0.f, 0, 3.f, false, true);
    idleHead->CreateKeyFrame(glm::vec3(0.15f, 0.f, 0.f), 0.f, 0.f, 0.f, 3.f, false, true);
    body->GetComponent<Component::Animation>()->Start("idle");

    Deactivate();
}

Rocket::~Rocket() {
    Resources().FreeOBJModel(mBodyModel);
    if (Active())
        Resources().FreeTexture2D(mDeactiveGlow);
    else
        Resources().FreeTexture2D(mActiveGlow);
}

float Rocket::GetHealth() {
    if (node->GetComponent<Component::GridCollide>()->hasCollided == true) {
        return 0.f;
    }
    return node->GetComponent<Component::Health>()->health;
}

void Rocket::Activate() {
    SuperEnemy::Activate();
    node->GetComponent<Component::Controller>()->enabled = true;
    body->GetComponent<Component::Material>()->glow = mActiveGlow;
}

void Rocket::Deactivate() {
    SuperEnemy::Deactivate();
    node->GetComponent<Component::Controller>()->enabled = false;
    body->GetComponent<Component::Material>()->glow = mDeactiveGlow;
    node->GetComponent<Component::Physics>()->acceleration = glm::vec3(0.f, 0.f, 0.f);
}

void Rocket::mUpdateFunction() {
    SuperEnemy::mUpdateFunction();
    Component::Physics* physics = node->GetComponent<Component::Physics>();
    float velocityFactor = glm::length(physics->velocity) / glm::length(physics->maxVelocity);
    physics->angularVelocity.y = velocityFactor;
    node->GetComponent<Component::Transform>()->pitch = -glm::min((2.f * velocityFactor) * 90.f, 90.f);
}
