#include "Portal.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include <Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include <Component/Transform.hpp>
#include <Component/PointLight.hpp>
#include <Component/Mesh.hpp>
#include <Component/Material.hpp>
#include <Component/Physics.hpp>
#include <Component/Animation.hpp>
#include <Component/ParticleEmitter.hpp>

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

Portal::Portal(Scene* scene) : SuperGameObject(scene) {
    node = CreateEntity();
    node->AddComponent<Component::Transform>()->scale *= 1.3f;
    node->AddComponent<Component::Physics>()->angularVelocity.y = 1.f;
    node->GetComponent<Component::Physics>()->angularDragFactor = 0.f;
    node->AddComponent<Component::Material>();
    node->GetComponent<Component::Material>()->SetDiffuse("Resources/color/defaultBlue.png");
    node->GetComponent<Component::Material>()->SetGlow("Resources/default/DefaultSpecular.png");
    node->AddComponent<Component::Mesh>()->geometry = mPortalModel = Resources().CreateOBJModel("Resources/portal/portal.obj");
    node->AddComponent<Component::Animation>();
    Component::Animation::AnimationClip* idleNode = node->GetComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleNode->CreateKeyFrame(glm::vec3(0.3f, 0.f, 0.f), 0.f, 0.f, 0, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(0.f, 0.1f, 0.1f), 0.f, 0.f, 0.f, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(0.1f, -0.1f, -0.1f), 0.f, 0.f, 0.f, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(-0.1f, 0.1f, 0.1f), 0.f, 0.f, 0.f, 1.5f, false, true);
    node->GetComponent<Component::Animation>()->Start("idle");
    node->AddComponent<Component::PointLight>()->attenuation = 1.5f;
    node->GetComponent<Component::PointLight>()->color = glm::vec3(109.f, 242.f, 207.f) * 0.1f;
}

Portal::~Portal() {
    Resources().FreeOBJModel(mPortalModel);
}
