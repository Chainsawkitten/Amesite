#include "HealthSystem.hpp"

#include <Engine/Scene/Scene.hpp>

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
        if (healthComponent->cooldown > 0.f) {
        } else {
            healthComponent->health += std::fminf(healthComponent->regenAmount * deltaTime, healthComponent->maxHealth);
        }
    }
}