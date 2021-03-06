#include "ReviveCircle.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Engine/Geometry/Geometry3D.hpp>
#include <Engine/Geometry/OBJModel.hpp>

#include "../Component/Health.hpp"
#include "../Component/Spawner.hpp"
#include "../Component/Explode.hpp"
#include "../Component/Controller.hpp"
#include "../Component/Update.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/Animation.hpp>
#include <Engine/Component/ParticleEmitter.hpp>
#include <Engine/Component/Physics.hpp>

#include "../Util/ControlSchemes.hpp"
#include "Player/SuperPlayer.hpp"

using namespace GameObject;

ReviveCircle::ReviveCircle(Scene* scene, SuperPlayer* player) : SuperGameObject(scene) {

    mPlayer = player;

    node = CreateEntity();
    node->AddComponent<Component::Transform>()->scale *= 1.f;
    node->AddComponent<Component::Update>()->updateFunction = std::bind(&ReviveCircle::mUpdateFunction, this);

    outerRing = CreateEntity();
    outerRing->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    outerRing->AddComponent<Component::Mesh>()->geometry = mRingModel = Resources().CreateOBJModel("Resources/reviveCircle/reviveCircle_ring.obj");
    outerRing->AddComponent<Component::Material>();
    outerRing->GetComponent<Component::Material>()->SetDiffuse("Resources/reviveCircle/reviveCircle_diff.png");
    outerRing->GetComponent<Component::Material>()->SetGlow("Resources/reviveCircle/reviveCircle_glow.png");
    outerRing->AddComponent<Component::Physics>()->angularDragFactor = 0.f;

    innerCircle = CreateEntity();
    innerCircle->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    innerCircle->AddComponent<Component::Mesh>()->geometry = mCircleModel = Resources().CreateOBJModel("Resources/reviveCircle/reviveCircle_circle.obj");
    innerCircle->AddComponent<Component::Material>();
    innerCircle->GetComponent<Component::Material>()->SetDiffuse("Resources/reviveCircle/reviveCircle_diff.png");
    innerCircle->GetComponent<Component::Material>()->SetGlow("Resources/reviveCircle/reviveCircle_glow.png");
}

ReviveCircle::~ReviveCircle() {
    Resources().FreeOBJModel(mRingModel);
    Resources().FreeOBJModel(mCircleModel);
}

void ReviveCircle::mUpdateFunction() {
    node->GetComponent<Component::Transform>()->position = mPlayer->GetPosition() - glm::vec3(0.f, 3.f, 0.f);
    float minFactor = 0.4f;
    float factor = 1.f - mPlayer->respawnTimeLeft / mPlayer->initalRespawnTime;
    factor = (1.f - minFactor) * factor * factor + minFactor;
    factor *= factor;
    innerCircle->GetComponent<Component::Transform>()->scale = glm::vec3(1.f, 1.f, 1.f) * factor;
    outerRing->GetComponent<Component::Physics>()->angularVelocity.y = 0.05f + factor * 0.5f;
    if (mPlayer->Active())
        Kill();
}
