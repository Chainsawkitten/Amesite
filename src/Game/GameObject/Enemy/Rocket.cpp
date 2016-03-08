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
    mActiveGlow = Resources().CreateTexture2DFromFile("Resources/enemy_head_crystal_glow.png");

    node->AddComponent<Component::Transform>()->scale *= 0.28f;
    node->AddComponent<Component::Collider2DCircle>()->radius = 9.f;
    node->AddComponent<Component::Physics>();
    node->AddComponent<Component::Health>()->faction = 1;
    node->GetComponent<Component::Health>()->health = 50.f;
    node->GetComponent<Component::Health>()->removeOnLowHealth = false;
    node->AddComponent<Component::Explode>()->lifeTime = 0.25f;
    node->GetComponent<Component::Explode>()->size = 8.f;
    node->GetComponent<Component::Explode>()->particleTextureIndex = Component::ParticleEmitter::PURPLE;
    node->GetComponent<Component::Explode>()->sound = true;
    node->AddComponent<Component::Update>()->updateFunction = std::bind(&Rocket::mUpdateFunction, this);

    head = CreateEntity();
    head->AddComponent<Component::RelativeTransform>()->Move(0, 0, 5.5f);
    head->GetComponent<Component::RelativeTransform>()->parentEntity = node;
    head->AddComponent<Component::Mesh>()->geometry = mEnemyHead = Resources().CreateOBJModel("Resources/enemy_head_crystal.obj");
    head->AddComponent<Component::Material>();
    head->GetComponent<Component::Material>()->SetDiffuse("Resources/enemy_head_crystal_diff.png");
    head->GetComponent<Component::Material>()->SetSpecular("Resources/enemy_head_crystal_spec.png");
    mDeactiveGlow = head->GetComponent<Component::Material>()->glow;
    Component::Animation::AnimationClip* idleHead = head->AddComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleHead->CreateKeyFrame(glm::vec3(-0.15f, 0.f, 0.f), 0.f, 0.f, 0, 3.f, false, true);
    idleHead->CreateKeyFrame(glm::vec3(0.15f, 0.f, 0.f), 0.f, 0.f, 0.f, 3.f, false, true);
    head->GetComponent<Component::Animation>()->Start("idle");

    tail = CreateEntity();
    tail->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    tail->GetComponent<Component::RelativeTransform>()->Move(0, 0, -5.5f);
    tail->AddComponent<Component::Mesh>()->geometry = mEnemyTail = Resources().CreateOBJModel("Resources/enemy_tail_crystal.obj");
    tail->AddComponent<Component::Material>();
    tail->GetComponent<Component::Material>()->SetDiffuse("Resources/enemy_tail_crystal_diff.png");
    tail->GetComponent<Component::Material>()->SetSpecular("Resources/enemy_tail_crystal_spec.png");
    tail->GetComponent<Component::Material>()->SetGlow("Resources/enemy_tail_crystal_glow.png");
    Component::Animation::AnimationClip* idleTail = tail->AddComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleTail->CreateKeyFrame(glm::vec3(-0.1f, 0.f, 0.f), 0.f, 0.f, 0, 2.f, false, true);
    idleTail->CreateKeyFrame(glm::vec3(0.1f, 0.f, 0.f), 0.f, 0.f, 0.f, 2.f, false, true);
    tail->GetComponent<Component::Animation>()->Start("idle");

    turret = CreateEntity();
    turret->AddComponent<Component::RelativeTransform>()->parentEntity = head;
    turret->GetComponent<Component::RelativeTransform>()->Move(0, 0, 8.f);
    turret->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AlwaysShoot);
    turret->AddComponent<Component::Spawner>()->delay = 0.5f;
    turret->GetComponent<Component::Spawner>()->faction = 1;

    Deactivate();
}

Rocket::~Rocket() {
    Resources().FreeOBJModel(mEnemyHead);
    Resources().FreeOBJModel(mEnemyTail);
    if (Active())
        Resources().FreeTexture2D(mDeactiveGlow);
    else
        Resources().FreeTexture2D(mActiveGlow);
}

float Rocket::GetHealth() {
    return node->GetComponent<Component::Health>()->health;
}

void Rocket::Activate() {
    SuperEnemy::Activate();
    turret->GetComponent<Component::Controller>()->enabled = true;
    tail->GetComponent<Component::Material>()->glow = mActiveGlow;
}

void Rocket::Deactivate() {
    SuperEnemy::Deactivate();
    turret->GetComponent<Component::Controller>()->enabled = false;
    tail->GetComponent<Component::Material>()->glow = mDeactiveGlow;
}

void Rocket::mUpdateFunction() {
    SuperEnemy::mUpdateFunction();
}
