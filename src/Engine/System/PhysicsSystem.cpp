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

void PhysicsSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::Physics*> physicsObjects;
    physicsObjects = scene.GetAll<Component::Physics>();
    for (unsigned int i = 0; i < physicsObjects.size(); i++) {
        Component::Physics* physicsComp = physicsObjects[i];
        Component::Transform* transformComp = physicsComp->entity->GetComponent<Component::Transform>();
        if (transformComp != nullptr) {

            // --- Velocity ---
            // Add acceleration.
            physicsComp->velocity += (physicsComp->acceleration + mGravity * physicsComp->gravityFactor) * deltaTime;

            // Add retardation.
            physicsComp->velocity -= physicsComp->velocity * physicsComp->velocityDragFactor * deltaTime;
            if (glm::length(physicsComp->velocity) > 0.01f) {
                // Cap velocity.
                if (glm::length(physicsComp->velocity) > physicsComp->maxVelocity)
                    physicsComp->velocity = glm::length(physicsComp->maxVelocity) / glm::length(physicsComp->velocity) * physicsComp->velocity;
            } else
                physicsComp->velocity = glm::vec3(0.f, 0.f, 0.f);

            // Update position.
            transformComp->position += physicsComp->velocity * deltaTime;     

            // --- Angular Velocity ---
            // Add angular acceleration.
            physicsComp->angularVelocity += physicsComp->angularAcceleration * physicsComp->momentOfInertia * deltaTime;
            
            // Add drag.
            physicsComp->angularVelocity -= physicsComp->angularVelocity * physicsComp->angularDragFactor * deltaTime;
            if (glm::length(physicsComp->angularVelocity) > 0.01f) {
                // Cap angular velocity.
                if (glm::length(physicsComp->angularAcceleration) > physicsComp->maxAngularVelocity)
                    physicsComp->angularAcceleration = glm::length(physicsComp->maxAngularVelocity) / glm::length(physicsComp->angularAcceleration) * physicsComp->angularAcceleration;
            } else
                physicsComp->angularAcceleration = glm::vec3(0.f, 0.f, 0.f);

            // Update rotation.
            transformComp->pitch += physicsComp->angularVelocity.x * (360.f) * deltaTime;
            transformComp->yaw += physicsComp->angularVelocity.y * (360.f) * deltaTime;
            transformComp->roll += physicsComp->angularVelocity.z * (360.f) * deltaTime;
        }
    }
}
