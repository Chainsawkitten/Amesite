#include "Altar.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include <Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../Component/Health.hpp"
#include "../Component/Spawner.hpp"
#include "../Component/Explode.hpp"
#include "../Component/Controller.hpp"
#include <Component/Transform.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/Mesh.hpp>
#include <Component/Material.hpp>
#include <Component/Collider2DCircle.hpp>
#include <Component/Animation.hpp>
#include <Component/ParticleEmitter.hpp>

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

Altar::Altar(Scene* scene) : SuperGameObject(scene) {
    node = CreateEntity();
    node->AddComponent<Component::Transform>()->scale *= 1.3f;
    node->GetComponent<Component::Transform>()->yaw = 20.f;
    node->AddComponent<Component::Material>();
    node->GetComponent<Component::Material>()->SetDiffuse("Resources/Altar_Dif.png");
    node->GetComponent<Component::Material>()->SetNormal("Resources/Altar_NM.png");
    node->GetComponent<Component::Material>()->SetGlow("Resources/Altar_Glo.png");
    node->AddComponent<Component::Mesh>()->geometry = mAltarModel = Resources().CreateOBJModel("Resources/Altar2.obj");
    //node->GetComponent<Component::Material>()->SetDiffuse("Resources/altar_diff.png");
    //node->GetComponent<Component::Material>()->SetGlow("Resources/altar_glow.png");
    //node->AddComponent<Component::Mesh>()->geometry = mAltarModel = Resources().CreateOBJModel("Resources/altar.obj");
}

Altar::~Altar() {
    Resources().FreeOBJModel(mAltarModel);
}
