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
#include <Component/SpotLight.hpp>

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
    light = CreateEntity();
    light->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    light->GetComponent<Component::Transform>()->scale *= 0.1;
    light->GetComponent<Component::Transform>()->Move(0.f, 22.f, 0.f);
    light->AddComponent<Component::SpotLight>();
    light->GetComponent<Component::SpotLight>()->attenuation = 1.5f;
    light->GetComponent<Component::SpotLight>()->coneAngle = 180.f;
    light->GetComponent<Component::SpotLight>()->color = glm::vec3(109.f, 242.f, 207.f) * 0.01f;
}

Altar::~Altar() {
    Resources().FreeOBJModel(mAltarModel);
}
