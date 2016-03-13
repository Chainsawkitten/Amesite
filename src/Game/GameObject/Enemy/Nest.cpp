#include "Nest.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../../Component/Health.hpp"
#include "../../Component/Spawner.hpp"
#include "../../Component/Explode.hpp"
#include "../../Component/Controller.hpp"
#include "../../Component/Update.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/Animation.hpp>
#include <Engine/Component/ParticleEmitter.hpp>

#include "../../Util/ControlSchemes.hpp"
#include "../../Util/GameEntityFactory.hpp"
#include "../Enemy/Rocket.hpp"

using namespace GameObject;

Nest::Nest(Scene* scene) : SuperEnemy(scene) {
    mActiveGlowBody = Resources().CreateTexture2DFromFile("Resources/enemy_glow.png");

    node->AddComponent<Component::Transform>()->scale *= 0.2f;
    node->AddComponent<Component::Collider2DCircle>()->radius = 9.f;
    node->AddComponent<Component::Health>()->faction = 1;
    node->GetComponent<Component::Health>()->health = 50.f;
    node->GetComponent<Component::Health>()->removeOnLowHealth = false;
    node->AddComponent<Component::Explode>()->lifeTime = 0.25f;
    node->GetComponent<Component::Explode>()->size = 8.f;
    node->GetComponent<Component::Explode>()->particleTextureIndex = Component::ParticleEmitter::PURPLE;
    node->GetComponent<Component::Explode>()->sound = true;
    node->GetComponent<Component::Explode>()->type = Component::Explode::ENEMY;
    node->AddComponent<Component::Update>()->updateFunction = std::bind(&Nest::mUpdateFunction, this);
    node->AddComponent<Component::Spawner>()->delay = 0.75f;
   

    body = CreateEntity();
    body->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    body->AddComponent<Component::Mesh>()->geometry = mBodyModel = Resources().CreateOBJModel("Resources/crystal.obj");
    body->AddComponent<Component::Material>();
    body->GetComponent<Component::Material>()->SetDiffuse("Resources/enemy_diff.png");
    body->GetComponent<Component::Material>()->SetSpecular("Resources/enemy_spec.png");
    mDeactiveGlowBody = body->GetComponent<Component::Material>()->glow;

    Deactivate();
}

Nest::~Nest() {
    Resources().FreeOBJModel(mBodyModel);
    if (Active()) {
        Resources().FreeTexture2D(mDeactiveGlowBody);
    } else {
        Resources().FreeTexture2D(mActiveGlowBody);
    }
}

float Nest::GetHealth() {
    return node->GetComponent<Component::Health>()->health;
}

void Nest::Activate() {
    SuperEnemy::Activate();
    body->GetComponent<Component::Material>()->glow = mActiveGlowBody;
}

void Nest::Deactivate() {
    SuperEnemy::Deactivate();
    body->GetComponent<Component::Material>()->glow = mDeactiveGlowBody;
}

void Nest::mUpdateFunction() {
    SuperEnemy::mUpdateFunction();
    if (Active()) {
        Component::Spawner* spawner = node->GetComponent<Component::Spawner>();
        if (spawner->timeSinceSpawn >= spawner->delay) {
            GameObject::Rocket* miniRocket = GameEntityCreator().CreateMiniRocket(node->GetComponent<Component::Transform>()->position);
            miniRocket->range = 2.f * this->range;
            spawner->timeSinceSpawn = 0.f;
        }
    }
}