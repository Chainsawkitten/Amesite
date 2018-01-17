#include "Shield.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include <Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>

#include "../Component/Health.hpp"
#include "../Component/Explode.hpp"
#include "../Component/LifeTime.hpp"
#include <Component/Transform.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/Mesh.hpp>
#include <Component/Material.hpp>
#include <Component/Collider2DCircle.hpp>
#include <Component/Animation.hpp>
#include <Component/ParticleEmitter.hpp>

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
    body->AddComponent<Component::Mesh>()->geometry = mShield = Resources().CreateOBJModel("Resources/shield/shield.obj");
    body->AddComponent<Component::Material>();
    body->GetComponent<Component::Material>()->SetDiffuse("Resources/shield/shield_diff.png");
    body->GetComponent<Component::Material>()->SetGlow("Resources/shield/shield_glow.png");
    body->GetComponent<Component::Material>()->SetSpecular("Resources/shield/shield_spec.png");
}

Shield::~Shield() {
    Resources().FreeOBJModel(mShield);
}
