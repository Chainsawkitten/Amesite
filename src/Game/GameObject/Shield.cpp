#include "Shield.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../Component/Health.hpp"
#include "../Component/Explode.hpp"
#include "../Component/LifeTime.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/Animation.hpp>
#include <Engine/Component/ParticleEmitter.hpp>

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

Shield::Shield(Scene* scene) : SuperGameObject(scene) {
    node = CreateEntity();
    node->AddComponent<Component::RelativeTransform>()->scale *= 2.f;
    node->AddComponent<Component::Animation>();

    body = CreateEntity();
    body->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    body->AddComponent<Component::Animation>();
    body->AddComponent<Component::Collider2DCircle>()->radius = 9.f;
    body->AddComponent<Component::LifeTime>();
    body->AddComponent<Component::Health>()->faction = 0;
    body->GetComponent<Component::Health>()->health = 50.f;
    body->AddComponent<Component::Mesh>()->geometry = mShield = Resources().CreateOBJModel("Resources/shield.obj");
    body->AddComponent<Component::Material>();
    body->GetComponent<Component::Material>()->SetDiffuse("Resources/shield_diff.png");
    body->GetComponent<Component::Material>()->SetGlow("Resources/shield_glow.png");
    body->GetComponent<Component::Material>()->SetSpecular("Resources/shield_spec.png");
}

Shield::~Shield() {
    Resources().FreeOBJModel(mShield);
}
