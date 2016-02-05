#include "Camera.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Lens.hpp>

using namespace GameObject;

Camera::Camera(Scene* scene) : SuperGameObject(scene) {
    Entity* entity = mScene->CreateEntity();
    mEntityMap["body"] = entity;
    entity->AddComponent<Component::Transform>();
    entity->AddComponent<Component::Physics>();
    entity->AddComponent<Component::Lens>();
    mEntityVector.push_back(entity);
}

Camera::~Camera() {
}
