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
    // Loop through collision vector
    for (auto collisionX : *collisionVector) {
        // Does the colliding entity have a health component or a reflect component?
        Component::Health* HealthX = collisionX->entity->GetComponent<Component::Health>();
        if (HealthX != nullptr && collisionX->entity->GetComponent<Component::Reflect>() == nullptr) {
            for (auto collisionY : collisionX->intersect) {
                // Does the intersecting entities have a damage component?
                Component::Damage* damageY = collisionY->GetComponent<Component::Damage>();
                if (damageY != nullptr) {
                    // Does the damaging if entity doesn't belong to the same faction as the health entity.
                    if (damageY->faction != HealthX->faction) {
                        // Reduce health by damage.
                        HealthX->health -= damageY->damageAmount;
                        // Remove damage entity if it should be removed on impact
                        if (damageY->removeOnImpact)
                            if (damageY->entity->gameObject != nullptr)
                                damageY->entity->gameObject->Kill();
                            else
                                damageY->entity->Kill();
                    }
                }
            }
        }
    }
}
