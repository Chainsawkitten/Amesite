#include "PhysicsSystem.hpp"

#include "../Scene/Scene.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/Transform.hpp"
#include "../Component/Physics.hpp"

#include "../Threading/Threading.hpp"

using namespace System;

PhysicsSystem::PhysicsSystem(glm::vec3 gravity) {
    mGravity = gravity;
}

PhysicsSystem::~PhysicsSystem() {
}

void PhysicsSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::Physics*> physicsObjects = scene.GetAll<Component::Physics>();
    
    mDeltaTime = deltaTime;
    Threading::ParallelFor(&PhysicsSystem::UpdatePart, this, physicsObjects);
    Threading::FrontEndJobs().Wait();
}

void PhysicsSystem::UpdatePart(std::vector<Component::Physics*>& physics, std::size_t begin, std::size_t length) {
    for (std::size_t i=begin; i < begin+length; ++i) {
        Component::Physics* physicsComp = physics[i];
        
        if (physicsComp->IsKilled())
            continue;
        
        Component::Transform* transformComp = physicsComp->entity->GetComponent<Component::Transform>();
        if (transformComp != nullptr) {
            
            // --- Velocity ---
            // Add acceleration.
            physicsComp->velocity += (physicsComp->acceleration + mGravity * physicsComp->gravityFactor) * mDeltaTime;
            
            // Add retardation.
            physicsComp->velocity -= physicsComp->velocity * physicsComp->velocityDragFactor * mDeltaTime;
            if (glm::length(physicsComp->velocity) > 0.01f) {
                // Cap velocity.
                if (glm::length(physicsComp->velocity) > physicsComp->maxVelocity)
                    physicsComp->velocity = glm::length(physicsComp->maxVelocity) / glm::length(physicsComp->velocity) * physicsComp->velocity;
            } else
                physicsComp->velocity = glm::vec3(0.f, 0.f, 0.f);
            
            // Update position.
            transformComp->position += physicsComp->velocity * mDeltaTime;
            
            // --- Angular Velocity ---
            // Add angular acceleration.
            physicsComp->angularVelocity += physicsComp->angularAcceleration * physicsComp->momentOfInertia * mDeltaTime;
            
            // Add drag.
            physicsComp->angularVelocity -= physicsComp->angularVelocity * physicsComp->angularDragFactor * mDeltaTime;
            if (glm::length(physicsComp->angularVelocity) > 0.01f) {
                // Cap angular velocity.
                if (glm::length(physicsComp->angularAcceleration) > physicsComp->maxAngularVelocity)
                    physicsComp->angularAcceleration = glm::length(physicsComp->maxAngularVelocity) / glm::length(physicsComp->angularAcceleration) * physicsComp->angularAcceleration;
            } else
                physicsComp->angularAcceleration = glm::vec3(0.f, 0.f, 0.f);
            
            // Update rotation.
            transformComp->pitch += physicsComp->angularVelocity.x * (360.f) * mDeltaTime;
            transformComp->yaw += physicsComp->angularVelocity.y * (360.f) * mDeltaTime;
            transformComp->roll += physicsComp->angularVelocity.z * (360.f) * mDeltaTime;
        }
    }
}
