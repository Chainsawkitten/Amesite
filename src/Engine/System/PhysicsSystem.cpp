#include "PhysicsSystem.hpp"

#include "../Scene/Scene.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/Transform.hpp"
#include "../Component/Physics.hpp"

using namespace System;

PhysicsSystem::PhysicsSystem(glm::vec3 gravity) {
    mGravity = gravity;
}

PhysicsSystem::~PhysicsSystem() {
}

void PhysicsSystem::Update(const Scene& scene, float timeDelta) {
    for (unsigned int i = 0; i < scene.Size<Component::Physics>(); i++) {
        Component::Physics* physicsComp = scene.Get<Component::Physics>(i);
        Component::Transform* transformComp = physicsComp->entity->GetComponent<Component::Transform>();
        if (transformComp != nullptr) {

            // --- Velocity ---
            // Add acceleration.
            physicsComp->velocity += (physicsComp->acceleration + mGravity * physicsComp->gravityCoefficient) * timeDelta;

            // Add retardation.
            if (glm::length(physicsComp->velocity) > 0.1f) {
                physicsComp->velocity += -glm::normalize(physicsComp->velocity) * glm::length(physicsComp->mass * glm::length(mGravity) * physicsComp->frictionCoefficient) * timeDelta;
            }
            else {
                physicsComp->velocity = glm::vec3(0.f);
            }

            // Cap velocity.
            if (glm::length(physicsComp->velocity) > physicsComp->maxVelocity) {
                physicsComp->velocity = glm::length(physicsComp->maxVelocity) / glm::length(physicsComp->velocity) * physicsComp->velocity;
            }

            // Update position.
            transformComp->position += physicsComp->velocity * timeDelta;

            // --- Angular Velocity ---
            // Add angular acceleration.
            physicsComp->angularVelocity += physicsComp->angularaAcceleration * physicsComp->momentOfInertia * timeDelta;
            
            // Add drag.
            if (glm::length(physicsComp->angularVelocity) > 0.01f) {
                physicsComp->angularVelocity -= physicsComp->angularVelocity * physicsComp->angularDragFactor * timeDelta;
            }
            else {
                physicsComp->angularVelocity  = glm::vec3(0.f);
            }

            // Cap angular velocity.
            if (glm::length(physicsComp->angularaAcceleration) > physicsComp->maxAngularVelocity) {

            }

            // Update rotation.
            transformComp->pitch += physicsComp->angularVelocity.x * (360.f) * timeDelta;
            transformComp->yaw += physicsComp->angularVelocity.y * (360.f) * timeDelta;
            transformComp->roll += physicsComp->angularVelocity.z * (360.f) * timeDelta;
        }
    }
}
