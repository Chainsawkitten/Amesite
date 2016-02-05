#include "DamageSystem.hpp"
#include <Engine/Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <Component/Physics.hpp>
#include "../Component/Health.hpp"
#include "../Component/Health.hpp"
#include "../Component/Damage.hpp"
#include "../Component/Controller.hpp"
#include <Component\Collider2DCircle.hpp>

#include <vector>

#include <Util/Log.hpp>

using namespace System;

DamageSystem::DamageSystem() {
}

DamageSystem::~DamageSystem() {
}

void DamageSystem::Update(Scene& scene) {
    std::vector<Scene::Collision*>* collisionVector = scene.GetVector<Scene::Collision>();
    int numberOfCollisions = collisionVector->size();
    for (int i = 0; i < numberOfCollisions; i++) {                                                                      //Loop through collision vector
        if((*collisionVector)[i]->entity->GetComponent<Component::Health>() != nullptr) {                               //Does the colliding entity have a health component?
            int numberOfIntersections = (*collisionVector)[i]->intersect.size();
            for (int j = 0; j < numberOfIntersections; j++) {
                if ( (*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>() != nullptr) {               //Does the intersecting entities have a damage component?
                    
                    Log() << (*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->faction << " : " << (*collisionVector)[i]->entity->GetComponent<Component::Health>()->faction << "\n";
                    
                    if ((*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->faction != (*collisionVector)[i]->entity->GetComponent<Component::Health>()->faction) {
                        //Does the damaging entity belong to the same faction as the health entity.
                        (*collisionVector)[i]->entity->GetComponent<Component::Health>()->health -= (*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->damageAmount;
                        (*collisionVector)[i]->intersect[j]->GetComponent<Component::Collider2DCircle>()->radius = 0.f;
                        (*collisionVector)[i]->intersect[j]->GetComponent<Component::Transform>()->position = glm::vec3(10000.f, 0, 0);
                        
                    }//Reduce health by damage.
                    
                                                                                                                                                                                            //scene.RemoveEntity((*collisionVector)[i]->entity);
                     //TODO: EXTREMELY TEMPORARY SOLUTION!
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
            if (health->entity->GetComponent<Component::Controller>() != nullptr && health->activated == true && health->faction == 0) {
                //TODO: Implement actual coop spawning
                health->entity->GetComponent<Component::Transform>()->Move(0.f, -0.5f, 0.f);
                health->entity->GetComponent<Component::Transform>()->scale = glm::vec3(0.5f, 0.5f, 0.5f);
                health->entity->GetComponent<Component::Physics>()->maxVelocity = 0.f;
                health->cooldown = 10.f;
                health->activated = false;
            }
            else if (health->entity->GetComponent<Component::Controller>() != nullptr && health->activated == false && health->faction == 0) {
                health->cooldown -= 0.1f;
                if (health->cooldown < 0.f) {
                    health->health = 100.f;
                    health->entity->GetComponent<Component::Transform>()->Move(0.f, 0.5f, 0.f);
                    health->entity->GetComponent<Component::Transform>()->scale = glm::vec3(1.0f, 1.0f, 1.0f);
                    health->entity->GetComponent<Component::Physics>()->maxVelocity = 20.f;
                    health->activated = true;
                }
            } else {
                //scene.RemoveEntity(health->entity);
                health->entity->GetComponent<Component::Transform>()->position = glm::vec3(1000.f, 1000.f, 1000.f);
                health->entity->GetComponent<Component::Collider2DCircle>()->radius = 0.f;
            }
        }
    }
}
