#include "Rock.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>


#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/PointLight.hpp>
#include <Engine/Component/Physics.hpp>
#include "../Component/Update.hpp"
#include "../Component/Spawner.hpp"
#include "../Component/LifeTime.hpp"

#include "../Util/GameEntityFactory.hpp"
#include "../Util/ControlSchemes.hpp"

#include "../GameObject/PillarBall.hpp"

using namespace GameObject;

Rock::Rock(Scene* scene) : SuperGameObject(scene) {

    node = CreateEntity();
    node->AddComponent<Component::Transform>();
    node->AddComponent<Component::Material>();
    node->GetComponent<Component::Material>()->SetDiffuse("Resources/pillar_diff.png");
    node->GetComponent<Component::Material>()->SetGlow("Resources/pillar_glow.png");
    node->AddComponent<Component::Mesh>()->geometry = mPillarModel = Resources().CreateOBJModel("Resources/pillar.obj");

    mLight = CreateEntity();
    mLight->AddComponent<Component::PointLight>();
    mLight->GetComponent<Component::PointLight>()->attenuation = 5.f;
    mLight->AddComponent<Component::RelativeTransform>()->parentEntity = node;

}

Rock::~Rock() {
    Resources().FreeOBJModel(mPillarModel);
}