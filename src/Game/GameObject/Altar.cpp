#include "Altar.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../Component/Health.hpp"
#include "../Component/Spawner.hpp"
#include "../Component/Explode.hpp"
#include "../Component/Controller.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/Animation.hpp>
#include <Engine/Component/ParticleEmitter.hpp>

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

Altar::Altar(Scene* scene) : SuperGameObject(scene) {
    node = CreateEntity();
    node->AddComponent<Component::Transform>();
    node->GetComponent<Component::Transform>()->yaw = 20.f;
    node->AddComponent<Component::Material>();
    node->GetComponent<Component::Material>()->SetDiffuse("Resources/altar_diff.png");
    node->GetComponent<Component::Material>()->SetGlow("Resources/altar_glow.png");
    node->AddComponent<Component::Mesh>()->geometry = mAltarModel = Resources().CreateOBJModel("Resources/altar.obj");

    //mPillarModel = Resources().CreateOBJModel("Resources/pillar.obj");

    //pillar1 = CreateEntity();
    //pillar1->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    //pillar1->GetComponent<Component::RelativeTransform>()->Move(25.f, 8.f, 15.f);
    //pillar1->GetComponent<Component::RelativeTransform>()->scale *= 2.f;
    //pillar1->GetComponent<Component::RelativeTransform>()->roll = 12.f;
    //pillar1->AddComponent<Component::Material>();
    //pillar1->GetComponent<Component::Material>()->SetDiffuse("Resources/pillar_diff.png");
    //pillar1->GetComponent<Component::Material>()->SetGlow("Resources/pillar_glow.png");
    //pillar1->AddComponent<Component::Mesh>()->geometry = mPillarModel;

    //pillar2 = CreateEntity();
    //pillar2->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    //pillar2->GetComponent<Component::RelativeTransform>()->Move(-25.f, 3.f, 15.f);
    //pillar2->GetComponent<Component::RelativeTransform>()->scale *= 2.5f;
    //pillar2->GetComponent<Component::RelativeTransform>()->yaw = -20.f;
    //pillar2->GetComponent<Component::RelativeTransform>()->roll = -16.f;
    //pillar2->AddComponent<Component::Material>();
    //pillar2->GetComponent<Component::Material>()->SetDiffuse("Resources/pillar_diff.png");
    //pillar2->GetComponent<Component::Material>()->SetGlow("Resources/pillar_glow.png");
    //pillar2->AddComponent<Component::Mesh>()->geometry = mPillarModel;
}

Altar::~Altar() {
    Resources().FreeOBJModel(mAltarModel);
}

