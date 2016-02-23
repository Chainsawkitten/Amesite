#include "DamageSystem.hpp"
#include <Engine/Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <Engine/GameObject/SuperGameObject.hpp>

#include "../Util/GameEntityFactory.hpp"

#include <Component/Physics.hpp>
#include <Component/ParticleEmitter.hpp>
#include "../Component/Health.hpp"
#include "../Component/Health.hpp"
#include "../Component/Damage.hpp"
#include "../Component/Explode.hpp"
#include <Component/Collider2DCircle.hpp>


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
    // Loop through collision vector
    for (int i = 0; i < numberOfCollisions; i++) {
        // Does the colliding entity have a health component?
        if ((*collisionVector)[i]->entity->GetComponent<Component::Health>() != nullptr) {
            int numberOfIntersections = (*collisionVector)[i]->intersect.size();
            for (int j = 0; j < numberOfIntersections; j++) {
                // Does the intersecting entities have a damage component?
                if ( (*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>() != nullptr) {
                    // Does the damaging if entity doesn't belong to the same faction as the health entity.
                    if ((*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->faction != (*collisionVector)[i]->entity->GetComponent<Component::Health>()->faction) {
                        // Reduce health by damage.
                        (*collisionVector)[i]->entity->GetComponent<Component::Health>()->health -= (*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->damageAmount;
                        // Remove damage entity if it should be removed on impact
                        if ((*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->removeOnImpact) {
                            // Create Explosion
                            Component::Explode* explodeComp = (*collisionVector)[i]->intersect[j]->GetComponent<Component::Explode>();
                            if (explodeComp != nullptr)
                                GameEntityCreator().CreateExplosion((*collisionVector)[i]->intersect[j]->GetComponent<Component::Transform>()->position, explodeComp->lifeTime, explodeComp->size, explodeComp->particleTextureIndex);
                            
                            // Remove game object
                            if ((*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->entity->gameObject != nullptr)
                                (*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->entity->gameObject->Clear();
                            else
                                (*collisionVector)[i]->intersect[j]->GetComponent<Component::Damage>()->entity->Clear();
                        }
                    }
                }
            }
        }
    }
}
