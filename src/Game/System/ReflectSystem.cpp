#include "ReflectSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include "../Component/Reflect.hpp"
#include <Engine/Component/Physics.hpp>

#include <vector>

using namespace System;

ReflectSystem::ReflectSystem() {
}

ReflectSystem::~ReflectSystem() {
}

void ReflectSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Scene::Collision*>* collisionVector = scene.GetVector<Scene::Collision>();
    for (auto collisionX : *collisionVector) {
        if (collisionX->entity->GetComponent<Component::Reflect>() != nullptr) {
            for (auto collisionY : collisionX->intersect) {
                Component::Physics* physicsY = collisionY->GetComponent<Component::Physics>();
                if (physicsY != nullptr) {
                    glm::vec3 normal = glm::normalize(collisionY->GetComponent<Component::Transform>()->GetWorldPosition() - collisionX->entity->GetComponent<Component::Transform>()->GetWorldPosition());
                    physicsY->velocity -= 2.f*(glm::dot(physicsY->velocity, normal)*normal);
                }
            }
        }
    }
}
