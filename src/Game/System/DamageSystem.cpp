#include "DamageSystem.hpp"
#include <Engine/Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <Engine/GameObject/SuperGameObject.hpp>

#include "../Util/GameEntityFactory.hpp"

#include <Component/Physics.hpp>
#include <Component/ParticleEmitter.hpp>
#include <Component/Collider2DCircle.hpp>
#include "../Component/Health.hpp"
#include "../Component/Reflect.hpp"
#include "../Component/Damage.hpp"
#include "../Component/Explode.hpp"



#include <vector>

#include <Util/Log.hpp>

using namespace System;

DamageSystem::DamageSystem() {
}

DamageSystem::~DamageSystem() {
}

void DamageSystem::Update(Scene& scene) {
    std::vector<Scene::Collision*>* collisionVector = scene.GetVector<Scene::Collision>();
    for (auto collisionX : *collisionVector) {
        Component::Health* HealthX = collisionX->entity->GetComponent<Component::Health>();
        if (HealthX != nullptr && collisionX->entity->GetComponent<Component::Reflect>() == nullptr) {
            for (auto collisionY : collisionX->intersect) {
                Component::Damage* damageY = collisionY->GetComponent<Component::Damage>();
                if (damageY != nullptr) {
                    if (damageY->faction != HealthX->faction) { //Does the damaging if entity doesn't belong to the same faction as the health entity.
                        HealthX->health -= damageY->damageAmount;   //Reduce health by damage.
                        if (damageY->removeOnImpact) {// Remove damage entity if it should be removed on impact
                            Component::Explode* explodeComp = collisionY->GetComponent<Component::Explode>();
                            if (explodeComp != nullptr)
                                GameEntityCreator().CreateExplosion(collisionY->GetComponent<Component::Transform>()->GetWorldPosition() + explodeComp->offset, explodeComp->lifeTime, explodeComp->size, explodeComp->particleTextureIndex); // Create Explosion
                            if (damageY->entity->gameObject != nullptr) // Remove game object
                                damageY->entity->gameObject->Clear();
                            else
                                damageY->entity->Clear();
                        }
                    }
                }
            }
        }
    }

    //std::vector<Scene::Collision*>* collisionVector = scene.GetVector<Scene::Collision>();
    //int numberOfCollisions = collisionVector->size();
    //for (int i = 0; i < numberOfCollisions; i++) {                                                                      //Loop through collision vector
    //    if((*collisionVector)[i]->entity->GetComponent<Component::Health>() != nullptr) {                               //Does the colliding entity have a health component?
    //        int numberOfIntersections = (*collisionVector)[i]->intersect.size();
    //        for (int j = 0; j < numberOfIntersections; j++) {
    //            if ( (*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>() != nullptr) {               //Does the intersecting entities have a damage component?
    //                if ((*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->faction != (*collisionVector)[i]->entity->GetComponent<Component::Health>()->faction) { //Does the damaging if entity doesn't belong to the same faction as the health entity.
    //                    (*collisionVector)[i]->entity->GetComponent<Component::Health>()->health -= (*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->damageAmount;   //Reduce health by damage.
    //                    if ((*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->removeOnImpact && (*collisionVector)[i]->intersect[j]->GetComponent<Component::Reflect>() == nullptr) {// Remove damage entity if it should be removed on impact
    //                        Component::Explode* explodeComp = (*collisionVector)[i]->intersect[j]->GetComponent<Component::Explode>();
    //                        if (explodeComp != nullptr)
    //                            GameEntityCreator().CreateExplosion((*collisionVector)[i]->intersect[j]->GetComponent<Component::Transform>()->GetWorldPosition()+explodeComp->offset, explodeComp->lifeTime, explodeComp->size, explodeComp->particleTextureIndex); // Create Explosion
    //                        if ((*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->entity->gameObject != nullptr) // Remove game object
    //                            (*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->entity->gameObject->Clear();
    //                        else
    //                            (*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->entity->Clear();
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}
}