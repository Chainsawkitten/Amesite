#include "HealthSystem.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <GameObject/SuperGameObject.hpp>

#include "../Component/Health.hpp"
#include "../Component/Explode.hpp"

#include "../Util/GameEntityFactory.hpp"

#include <Threading/Threading.hpp>

using namespace System;

HealthSystem::HealthSystem() {
}

HealthSystem::~HealthSystem() {
}

void HealthSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::Health*>& healthComponents = scene.GetAll<Component::Health>();
    
    mDeltaTime = deltaTime;
    Threading::ParallelFor(&HealthSystem::UpdatePart, this, healthComponents);
    Threading::FrontEndJobs().Wait();
}

void HealthSystem::UpdatePart(std::vector<Component::Health*>& healths, std::size_t begin, std::size_t length) {
    for (std::size_t i=begin; i < begin+length; ++i) {
        Component::Health* healthComponent = healths[i];
        
        // Update last hit time
        healthComponent->cooldown = std::fmaxf(healthComponent->cooldown - mDeltaTime, 0.f);
        
        // Update hp
        if (healthComponent->cooldown < 0.01f)
            healthComponent->health = std::fminf(healthComponent->health + healthComponent->regenAmount * mDeltaTime, healthComponent->maxHealth);
        if (healthComponent->health < 0.01f && healthComponent->removeOnLowHealth)
            if (healthComponent->health < 0.01f && healthComponent->removeOnLowHealth)
                if (healthComponent->entity->gameObject != nullptr)
                    healthComponent->entity->gameObject->Kill();
                else
                    healthComponent->entity->Kill();
    }
}
