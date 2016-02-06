#include "Bullet.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/Cube.hpp>
#include "../Component/Damage.hpp"
#include "../Component/LifeTime.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Collider2DCircle.hpp>

using namespace GameObject;

Bullet::Bullet(Scene* scene) : SuperGameObject(scene) {
    Entity* entity = mScene->CreateEntity();
    mEntityMap["body"] = entity;
    entity->AddComponent<Component::Damage>()->damageAmount = 10.f;
    entity->AddComponent<Component::Transform>()->scale = glm::vec3(0.5f, 0.5f, 0.5f);
    entity->AddComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    entity->AddComponent<Component::Collider2DCircle>()->radius = 0.25f;
    entity->AddComponent<Component::Physics>();
    entity->AddComponent<Component::LifeTime>()->lifeTime = 5.f;
    mEntityVector.push_back(entity);
}

Bullet::~Bullet() {
    Resources().FreeCube();
}
