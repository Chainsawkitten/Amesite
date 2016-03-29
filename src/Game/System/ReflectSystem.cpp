#include "ReflectSystem.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include "../Component/Reflect.hpp"
#include "../Component/Damage.hpp"
#include "../Component/LifeTime.hpp"
#include <Component/Physics.hpp>

#include <vector>

using namespace System;

ReflectSystem::ReflectSystem() {
}

ReflectSystem::~ReflectSystem() {
}

void ReflectSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Scene::Collision*>* collisionVector = scene.GetVector<Scene::Collision>();
    for (auto collisionX : *collisionVector) {
        Component::Reflect* reflectCompX = collisionX->entity->GetComponent<Component::Reflect>();
        if (reflectCompX != nullptr) {
            for (auto collisionY : collisionX->intersect) {
                Component::Damage* damageCompY = collisionY->GetComponent<Component::Damage>();
                if (damageCompY != nullptr) {
                    if (reflectCompX->faction != damageCompY->faction) {
                        Component::Physics* physicsY = collisionY->GetComponent<Component::Physics>();
                        if (physicsY != nullptr && damageCompY != nullptr) {
                            glm::vec3 normal = glm::normalize(collisionY->GetComponent<Component::Transform>()->GetWorldPosition() - collisionX->entity->GetComponent<Component::Transform>()->GetWorldPosition());
                            physicsY->velocity -= 2.f*(glm::dot(physicsY->velocity, normal)*normal);
                            damageCompY->faction = reflectCompX->faction;
                            Component::LifeTime* lifeTimeComponent = collisionY->GetComponent<Component::LifeTime>();
                            if (lifeTimeComponent != nullptr)
                                lifeTimeComponent->lifeTime = lifeTimeComponent->initialLifeTime;
                        }
                    }
                }
            }
        }
    }
}
