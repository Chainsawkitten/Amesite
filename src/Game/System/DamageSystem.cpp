#include "DamageSystem.hpp"

#include <Engine/Scene/Scene.hpp>

#include "../Component/Damage.hpp"

#include <vector>

using namespace System;

DamageSystem::DamageSystem() {
}

DamageSystem::~DamageSystem() {
}

void DamageSystem::Update(Scene& scene) {
    std::vector<Component::Damage*> damageVector = scene.GetAll<Component::Damage>();
    std::vector<Scene::Collision*>* collisionVector = scene.GetVector<Scene::Collision>();
    
    // TODO: CHECK COLLISION BETWEEN HEALTH AND DAMAGE COMPONENT

    /* 
    IF HIT
    healthComponent->cooldown = healthComponent->maxCooldown;
    healthComponent->health -= healthComponent->toughness * damageComponenet->damageAmount;
    */

    for (auto damageComponent : damageVector) {

    }
}
