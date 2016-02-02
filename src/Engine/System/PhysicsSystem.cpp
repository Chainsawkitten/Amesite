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

void PhysicsSystem::Update(const Scene& scene, float deltaTime) {
    for (unsigned int i = 0; i < scene.Size<Component::Physics>(); i++) {
        Component::Physics* physicsComp = scene.Get<Component::Physics>(i);
        Component::Transform* transformComp = physicsComp->entity->GetComponent<Component::Transform>();
        if (transformComp != nullptr) {

            // --- Velocity ---
            // Add acceleration.
            physicsComp->velocity += (physicsComp->acceleration + mGravity * physicsComp->gravityFactor) * deltaTime;

            // Add retardation.
            if (glm::length(physicsComp->velocity) > 0.5f)
                physicsComp->velocity -=  physicsComp->velocity * physicsComp->velocityDragFactor * deltaTime;

            // Cap velocity.
            if (glm::length(physicsComp->velocity) > physicsComp->maxVelocity)
                physicsComp->velocity = glm::length(physicsComp->maxVelocity) / glm::length(physicsComp->velocity) * physicsComp->velocity;

            // Update position.
            transformComp->position += physicsComp->velocity * deltaTime;

            // --- Angular Velocity ---
            // Add angular acceleration.
            physicsComp->angularVelocity += physicsComp->angularaAcceleration * physicsComp->momentOfInertia * deltaTime;
            
            // Add drag.
            if (glm::length(physicsComp->angularVelocity) > 0.01f)
                physicsComp->angularVelocity -= physicsComp->angularVelocity * physicsComp->angularDragFactor * deltaTime;

            // Cap angular velocity.
            if (glm::length(physicsComp->angularaAcceleration) > physicsComp->maxAngularVelocity)
                physicsComp->angularaAcceleration = glm::length(physicsComp->maxAngularVelocity) / glm::length(physicsComp->angularaAcceleration) * physicsComp->angularaAcceleration;

            // Update rotation.
            transformComp->pitch += physicsComp->angularVelocity.x * (360.f) * deltaTime;
            transformComp->yaw += physicsComp->angularVelocity.y * (360.f) * deltaTime;
            transformComp->roll += physicsComp->angularVelocity.z * (360.f) * deltaTime;
        }
    }
}
