#include "DamageSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/GameObject/SuperGameObject.hpp>

#include <../Game/GameObject/DyingLight.hpp>

#include "../Util/GameEntityFactory.hpp"

#include <Engine/Component/Physics.hpp>
#include <Engine/Component/ParticleEmitter.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/PointLight.hpp>
#include "../Component/Health.hpp"
#include "../Component/Reflect.hpp"
#include "../Component/Damage.hpp"
#include "../Component/Explode.hpp"
#include "../Component/LifeTime.hpp"
#include "../Component/GridCollide.hpp"

#include <vector>

#include <Engine/Util/Log.hpp>

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
                        HealthX->cooldown = HealthX->maxCooldown;
                        if (HealthX->damaged == -1)
                            HealthX->damaged = 1;
                        
                        // Remove damage entity if it should be removed on impact
                        if (damageY->removeOnImpact) {
                            Component::Explode* explosionComponent = damageY->entity->GetComponent<Component::Explode>();

                            if ( explosionComponent != nullptr) {

                                explosionComponent->size += 10.f;
                                GameObject::DyingLight* dyingLight = new GameObject::DyingLight(&scene);
                                dyingLight->node->GetComponent<Component::Transform>()->position = damageY->entity->GetComponent<Component::Transform>()->GetWorldPosition();
                                explosionComponent->type = Component::Explode::TYPE::ENEMY;
                            }

                            if (damageY->entity->gameObject != nullptr)
                                damageY->entity->gameObject->Kill();
                            else
                                damageY->entity->Kill();

                        }
                        
                        // Set the damage entity to have collided if it has a GridCollide component.
                        Component::GridCollide* gridCollide = collisionY->GetComponent<Component::GridCollide>();
                        if (gridCollide != nullptr)
                            gridCollide->hasCollided = true;
                    }
                }
            }
        }
    }
}
