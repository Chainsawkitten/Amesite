#include "DivideBoss.hpp"

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
#include "../../Component/LifeTime.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/Animation.hpp>
#include <Engine/Component/ParticleEmitter.hpp>
#include <Engine/Component/Physics.hpp>

#include "../../Util/ControlSchemes.hpp"
#include "../../Util/GameEntityFactory.hpp"
#include "../Enemy/Rocket.hpp"

using namespace GameObject;

DivideBoss::DivideBoss(Scene* scene) : SuperBoss(scene) {
    node->AddComponent<Component::Transform>()->scale *= 1.f;
    node->AddComponent<Component::Update>()->updateFunction = std::bind(&DivideBoss::mUpdateFunction, this);

    body->GetComponent<Component::Transform>()->scale *= 1.2f;
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
    body->GetComponent<Component::Health>()->health = body->GetComponent<Component::Health>()->maxHealth = 300.f;
    body->AddComponent<Component::Physics>()->angularDragFactor = 0.f;
    body->GetComponent<Component::Physics>()->angularVelocity.y = -0.1f;

    mNextHpStep = 0.9f;
}

DivideBoss::~DivideBoss() {
    Resources().FreeOBJModel(mBodyModel);
}

float DivideBoss::GetHealth() {
    return body->GetComponent<Component::Health>()->health;
}

glm::vec3 DivideBoss::GetPosition() {
    return node->GetComponent<Component::Transform>()->position;
}

void DivideBoss::Activate() {
    SuperBoss::Activate();
}

void DivideBoss::Deactivate() {
    SuperBoss::Deactivate();
}

void DivideBoss::mUpdateFunction() {
    SuperBoss::mUpdateFunction();
    Component::Health* heathComp = body->GetComponent<Component::Health>();
    float healthFactor = heathComp->health / heathComp->maxHealth;
    if (healthFactor < mNextHpStep) {
        mNextHpStep -= 0.1f;
        for (int i = 0; i < 5; i++) {
            std::uniform_int_distribution<uint32_t> randomValue(0, 1);
            GameObject::Rocket* miniRocket = GameEntityCreator().CreateMiniRocket(body->GetComponent<Component::Transform>()->GetWorldPosition() + glm::vec3(randomValue(mRNG) * 2.f - 1.f, 0.f, randomValue(mRNG) * 2.f - 1.f) * 10.f);
            miniRocket->range = this->range;
            miniRocket->node->GetComponent<Component::LifeTime>()->lifeTime = 20.f;
        }
    }
    body->GetComponent<Component::Transform>()->scale = glm::vec3(1.f, 1.f, 1.f) * healthFactor + 0.2f;
}
