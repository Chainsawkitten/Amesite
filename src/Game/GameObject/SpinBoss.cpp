#include "SpinBoss.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../Component/Health.hpp"
#include "../Component/Explode.hpp"
#include "../Component/LifeTime.hpp"
#include "../Component/Reflect.hpp"
#include "../Component/Controller.hpp"
#include "../Component/Spawner.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/Animation.hpp>
#include <Engine/Component/ParticleEmitter.hpp>
#include <Engine/Component/Physics.hpp>

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

SpinBoss::SpinBoss(Scene* scene) : SuperGameObject(scene) {
    node = CreateEntity(scene);
    node->AddComponent<Component::Transform>()->scale *= 1.f;
    node->AddComponent<Component::Physics>()->angularDragFactor = 0.f;
    node->GetComponent<Component::Physics>()->angularVelocity.y = -0.1f;

    body = CreateEntity(scene);
    body->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    body->AddComponent<Component::Mesh>()->geometry = mBody = Resources().CreateOBJModel("Resources/Crystal_01.obj");
    body->AddComponent<Component::Material>();
    body->GetComponent<Component::Material>()->SetDiffuse("Resources/Crystal_01_diff.png");
    body->GetComponent<Component::Material>()->SetSpecular("Resources/Crystal_01_spec.png");
    body->GetComponent<Component::Material>()->SetGlow("Resources/Crystal_01_glow.png");
    body->AddComponent<Component::Reflect>();
    body->AddComponent<Component::Collider2DCircle>()->radius = 6.f;
    body->AddComponent<Component::Explode>()->size = 1000.f;
    body->GetComponent<Component::Explode>()->particleTextureIndex = Component::ParticleEmitter::PURPLE;
    body->GetComponent<Component::Explode>()->lifeTime = 1.5f;
    body->GetComponent<Component::Explode>()->offset.y = 5.0f;

    for (int i = 0; i < 4; i++) {
        armArr[i] = CreateEntity(scene);
        armArr[i]->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    }
    CreateArm(armArr[0], glm::vec3(0, 0, 15));
    CreateArm(armArr[1], glm::vec3(15, 0, 0));
    CreateArm(armArr[2], glm::vec3(0, 0, -15));
    CreateArm(armArr[3], glm::vec3(-15, 0, 0));

    firstPhase = true;
}

SpinBoss::~SpinBoss() {
    Resources().FreeOBJModel(mBody);
}

void SpinBoss::Update() {
    if (firstPhase) {
        bool nextPhase = true;
        for (int i = 0; i < 4 && firstPhase; i++)
            if (armArr[i] != nullptr) {
                nextPhase = false;
                if (armArr[i]->GetComponent<Component::Health>()->health < 0.01f) {
                    armArr[i]->Clear();
                    armArr[i] = nullptr;
                }
            }
        firstPhase = !nextPhase;
        if (nextPhase) {
            body->AddComponent<Component::Health>()->faction = 1;
            body->GetComponent<Component::Material>()->SetGlow("Resources/Crystal_01_spec.png");
            body->RemoveComponent<Component::Reflect>();
        }
    } 
}

void SpinBoss::Reset() {
    for (int i = 0; i < 4; i++)
        if (armArr[i] != nullptr)
                armArr[i]->Clear();

    CreateArm(armArr[0], glm::vec3(0, 0, 15));
    CreateArm(armArr[1], glm::vec3(15, 0, 0));
    CreateArm(armArr[2], glm::vec3(0, 0, -15));
    CreateArm(armArr[3], glm::vec3(-15, 0, 0));

    firstPhase = true;
}

void SpinBoss::CreateArm(Entity* entity, glm::vec3 direction) {
    entity->GetComponent<Component::RelativeTransform>()->Move(direction);
    entity->GetComponent<Component::RelativeTransform>()->scale *= 0.5f;
    entity->AddComponent<Component::Physics>()->angularDragFactor = 0.f;
    entity->GetComponent<Component::Physics>()->angularVelocity.y = 0.25f;
    entity->AddComponent<Component::Collider2DCircle>()->radius = 6.f;
    entity->AddComponent<Component::Health>()->removeOnLowHealth = false;
    entity->AddComponent<Component::Health>()->faction = 1;
    entity->AddComponent<Component::Mesh>()->geometry = mBody;
    entity->AddComponent<Component::Material>();
    entity->GetComponent<Component::Material>()->SetDiffuse("Resources/Crystal_01_diff.png");
    entity->GetComponent<Component::Material>()->SetSpecular("Resources/Crystal_01_spec.png");
    entity->GetComponent<Component::Material>()->SetGlow("Resources/Crystal_01_glow.png");
    entity->AddComponent<Component::Controller>()->controlSchemes.push_back(ControlScheme::AlwaysShoot);
    entity->AddComponent<Component::Spawner>()->faction = 1;
    entity->GetComponent<Component::Spawner>()->delay = 0.2f;
}