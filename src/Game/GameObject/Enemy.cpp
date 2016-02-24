#include "Enemy.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../Component/Controller.hpp"
#include "../Component/Health.hpp"
#include "../Component/Spawner.hpp"
#include "../Component/Explode.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/Animation.hpp>
#include <Engine/Component/ParticleEmitter.hpp>

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

Enemy::Enemy(Scene* scene) : SuperGameObject(scene) {
    node = CreateEntity(scene);
    node->AddComponent<Component::Transform>()->scale *= 0.28f;
    node->AddComponent<Component::Collider2DCircle>()->radius = 9.f;
    node->AddComponent<Component::Physics>();
    node->AddComponent<Component::Health>()->faction = 1;
    node->GetComponent<Component::Health>()->health = 50.f;
    node->AddComponent<Component::Explode>()->lifeTime = 0.25f;
    node->GetComponent<Component::Explode>()->size = 8.f;
    node->GetComponent<Component::Explode>()->particleTextureIndex = Component::ParticleEmitter::PURPLE;

    head = CreateEntity(scene);
    head->AddComponent<Component::RelativeTransform>()->Move(0, 0, 5.5f);
    head->GetComponent<Component::RelativeTransform>()->parentEntity = node;
    head->AddComponent<Component::Mesh>()->geometry = mEnemyHead = Resources().CreateOBJModel("Resources/enemy_head_crystal.obj");
    head->AddComponent<Component::Material>();
    head->GetComponent<Component::Material>()->SetDiffuse("Resources/enemy_head_crystal_diff.png");
    head->GetComponent<Component::Material>()->SetSpecular("Resources/enemy_head_crystal_spec.png");
    head->GetComponent<Component::Material>()->SetGlow("Resources/enemy_head_crystal_glow.png");
    Component::Animation::AnimationClip* idleHead = head->AddComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleHead->CreateKeyFrame(glm::vec3(-0.15f, 0.f, 0.f), 0.f, 0.f, 0, 3.f, false, true);
    idleHead->CreateKeyFrame(glm::vec3(0.15f, 0.f, 0.f), 0.f, 0.f, 0.f, 3.f, false, true);
    head->GetComponent<Component::Animation>()->Start("idle");

    tail = CreateEntity(scene);
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

    turrent = CreateEntity(scene);
    turrent->AddComponent<Component::RelativeTransform>()->parentEntity = head;
    turrent->GetComponent<Component::RelativeTransform>()->Move(0, 0, 8.f);
    turrent->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AlwaysShoot);
    turrent->AddComponent<Component::Spawner>()->delay = 0.5f;
    turrent->GetComponent<Component::Spawner>()->faction = 1;
}

Enemy::~Enemy() {
    Resources().FreeOBJModel(mEnemyHead);
    Resources().FreeOBJModel(mEnemyTail);
}

float Enemy::GetHealth() {
    return node->GetComponent<Component::Health>()->health;
}
