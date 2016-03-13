#include "GroundCollideSystem.hpp"

#include "../GameObject/Cave.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <vector>

#include "../Component/GroundCollide.hpp"

using namespace System;

GroundCollideSystem::GroundCollideSystem() {
}

GroundCollideSystem::~GroundCollideSystem() {
}

void GroundCollideSystem::Update(Scene& scene, float deltaTime, GameObject::Cave& cave) {
    std::vector<Component::GroundCollide*> groundCollideVector = scene.GetAll<Component::GroundCollide>();
    for (auto& groundCollideComponent : groundCollideVector) {
        Component::Transform* transform = groundCollideComponent->entity->GetComponent<Component::Transform>();
        if (transform != nullptr) {
            glm::vec3 worldPosition = transform->GetWorldPosition();
            float terrainHeight = cave.GetTerrainHeight(worldPosition.x, worldPosition.z) - 10.f;
            if (worldPosition.y < terrainHeight) {
                groundCollideComponent->hasCollided = true;
                worldPosition.y = terrainHeight;
                if (groundCollideComponent->removeOnImpact) {
                    if (groundCollideComponent->entity->gameObject != nullptr)
                        groundCollideComponent->entity->gameObject->Kill();
                    else
                        groundCollideComponent->entity->Kill();
                }
            }
        }
    }
}
