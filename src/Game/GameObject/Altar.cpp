#include "Altar.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Engine/Geometry/Geometry3D.hpp>
#include <Engine/Geometry/OBJModel.hpp>

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
#include <Engine/Component/SpotLight.hpp>

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

Altar::Altar(Scene* scene) : SuperGameObject(scene) {
    node = CreateEntity();
    node->AddComponent<Component::Transform>()->scale *= 1.3f;
    node->GetComponent<Component::Transform>()->yaw = 20.f;
    node->AddComponent<Component::Material>();
    node->GetComponent<Component::Material>()->SetDiffuse("Resources/spawn/altar/Altar_Dif.png");
    node->GetComponent<Component::Material>()->SetNormal("Resources/spawn/altar/Altar_NM.png");
    node->GetComponent<Component::Material>()->SetGlow("Resources/spawn/altar/Altar_Glo.png");
    node->AddComponent<Component::Mesh>()->geometry = mAltarModel = Resources().CreateOBJModel("Resources/spawn/altar/Altar2.obj");

    light = CreateEntity();
    light->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    light->GetComponent<Component::Transform>()->scale *= 0.1;
    light->GetComponent<Component::Transform>()->Move(0.f, 22.f, 0.f);
    light->AddComponent<Component::SpotLight>();
    light->GetComponent<Component::SpotLight>()->attenuation = 1.5f;
    light->GetComponent<Component::SpotLight>()->coneAngle = 180.f;
    light->GetComponent<Component::SpotLight>()->color = glm::vec3(109.f, 242.f, 207.f) * 0.02f;
}

Altar::~Altar() {
    Resources().FreeOBJModel(mAltarModel);
}
