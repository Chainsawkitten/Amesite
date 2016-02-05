#include "DamageSystem.hpp"
#include <Engine/Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include "../Component/Health.hpp"
#include "../Component/Damage.hpp"
#include <vector>

using namespace System;

DamageSystem::DamageSystem() {
}

DamageSystem::~DamageSystem() {
}

void DamageSystem::Update(Scene& scene) {
    std::vector<Scene::Collision*>* collisionVector = scene.GetVector<Scene::Collision>();
    int numberOfCollisions = collisionVector->size();
    for (int i = 0; i < numberOfCollisions; i++) {
        if((*collisionVector)[i]->entity->GetComponent<Component::Health>() != nullptr) {
            int numberOfIntersections = (*collisionVector)[i]->intersect.size();
            for (int j = 0; j < numberOfIntersections; j++) {
                if ( (*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>() != nullptr) {
                    (*collisionVector)[i]->entity->GetComponent<Component::Health>()->health -= (*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->damageAmount;
                    //scene.RemoveEntity((*collisionVector)[i]->entity);
                }
            }
        }
    }

    // TODO: CHECK COLLISION BETWEEN HEALTH AND DAMAGE COMPONENT

    /* 
    IF HIT
    healthComponent->cooldown = healthComponent->maxCooldown;
    healthComponent->health -= healthComponent->toughness * damageComponenet->damageAmount;
    */

    for (auto health : scene.GetAll<Component::Health>()) {
        if (health->health < 0.f) {
            scene.RemoveEntity(health->entity);
        }
    }
}
