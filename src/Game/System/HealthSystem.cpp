#include "HealthSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/GameObject/SuperGameObject.hpp>

#include "../Component/Health.hpp"

#include <vector>

using namespace System;

HealthSystem::HealthSystem() {
}

HealthSystem::~HealthSystem() {
}

void HealthSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::Health*> healthVector = scene.GetAll<Component::Health>();
    for (auto healthComponent : healthVector) {
        // Update last hit time
        healthComponent->cooldown = std::fmaxf(healthComponent->cooldown - deltaTime, 0.f);
        // Update hp
        if (healthComponent->cooldown < 0.01f) 
            healthComponent->health += std::fminf(healthComponent->regenAmount * deltaTime, healthComponent->maxHealth);
        if (healthComponent->health < 0.01f && healthComponent->removeOnLowHealth) {
            if (healthComponent->health < 0.01f && healthComponent->removeOnLowHealth) {
                //NEVER REMOVE AN ENITY THAT GOT RELATIVE TRANSFROM, WILL LEAD TO EMPTY POINTERS
                if (healthComponent->entity->gameObject != nullptr)
                    healthComponent->entity->gameObject->Clear();
                else
                    healthComponent->entity->Clear();
            }
        }
    }
}
