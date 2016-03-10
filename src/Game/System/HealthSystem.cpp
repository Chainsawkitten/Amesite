#include "HealthSystem.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <GameObject/SuperGameObject.hpp>

#include "../Component/Health.hpp"
#include "../Component/Explode.hpp"

#include "../Util/GameEntityFactory.hpp"

#include <vector>

using namespace System;

HealthSystem::HealthSystem() {
}

HealthSystem::~HealthSystem() {
}

void HealthSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::Health*> healthComponents = scene.GetAll<Component::Health>();
    for (auto healthComponent : healthComponents) {
        // Update last hit time
        healthComponent->cooldown = std::fmaxf(healthComponent->cooldown - deltaTime, 0.f);
        // Update hp
        if (healthComponent->cooldown < 0.01f)
            healthComponent->health = std::fminf(healthComponent->health + healthComponent->regenAmount * deltaTime, healthComponent->maxHealth);
        if (healthComponent->health < 0.01f && healthComponent->removeOnLowHealth)
            if (healthComponent->health < 0.01f && healthComponent->removeOnLowHealth)
                if (healthComponent->entity->gameObject != nullptr)
                    healthComponent->entity->gameObject->Kill();
                else
                    healthComponent->entity->Kill();

        //Update damaged
        if (healthComponent->damaged != -1) {

            healthComponent->damaged--;

        } 
    }
}
