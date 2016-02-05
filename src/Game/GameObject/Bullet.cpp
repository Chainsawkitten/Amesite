#include "Bullet.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/Cube.hpp>

#include "../Component/Damage.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Collider2DCircle.hpp>

using namespace GameObject;

Bullet::Bullet(Scene* scene) : SuperGameObject(scene) {
    Entity* entity = mScene->CreateEntity();
    mEntityMap["body"] = entity;
    entity->AddComponent<Component::Damage>();
    entity->AddComponent<Component::Transform>()->position.x = 2.f;
    entity->AddComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    entity->AddComponent<Component::Collider2DCircle>()->radius = 0.5;
    entity->AddComponent<Component::Physics>()->angularAcceleration.y = 0.2f;
    mEntityVector.push_back(entity);
}

Bullet::~Bullet() {
    Resources().FreeCube();
}
