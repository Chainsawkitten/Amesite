#include "DamageSystem.hpp"
#include <Engine/Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <Component/Physics.hpp>
#include "../Component/Health.hpp"
#include "../Component/Health.hpp"
#include "../Component/Damage.hpp"
#include "../Component/Controller.hpp"
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

    IF health < 0
    remove entity(health)
    remove enityt(damage)
    */

    for (auto health : scene.GetAll<Component::Health>()) {
        if (health->health < 0.f) {
            // If the killed entity is a player, do other stuff.
            if (health->entity->GetComponent<Component::Controller>() != nullptr) {
                if(health->cooldown < 0.f){
                    //TODO: Implement actual coop spawning
                    health->entity->GetComponent<Component::Transform>()->Move(0.f, -0.5f, 0.f);
                    health->entity->GetComponent<Component::Transform>()->scale = glm::vec3(0.5f, 0.5f, 0.5f);
                    health->entity->GetComponent<Component::Physics>()->velocityDragFactor = 0.1f;
                    health->cooldown = 10.f;
                } else if (health->cooldown > 0.f) {

                }
            }
                
            scene.RemoveEntity(health->entity);
        }
    }
}
