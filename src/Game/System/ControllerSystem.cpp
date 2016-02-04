#include "ControllerSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Physics.hpp>

#include "../Component/Controller.hpp"
#include "../Component/Spawner.hpp"
#include <Util/Log.hpp>
#include "../Util/GameEntityFactory.hpp"

using namespace System;

void ControllerSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::Controller*> controllerObjects;
    controllerObjects = scene.GetAll<Component::Controller>();
    
    for (unsigned int i = 0; i < controllerObjects.size(); i++) {
        Component::Transform* transformComponent = controllerObjects[i]->entity->GetComponent<Component::Transform>();
        
        // Move the player
        float x = Input()->ButtonValue(Input()->MOVE_X, controllerObjects[i]->playerID);
        float z = Input()->ButtonValue(Input()->MOVE_Z, controllerObjects[i]->playerID);
        
        Component::Spawner* spawnerComponent = controllerObjects[i]->entity->GetComponent<Component::Spawner>();
        if (spawnerComponent != nullptr) {
            spawnerComponent->timeSinceSpawn += deltaTime;
            if (Input()->Pressed(Input()->SHOOT, controllerObjects[i]->playerID) && spawnerComponent->timeSinceSpawn >= spawnerComponent->delay) {
                GameEntityCreator().SetScene(&scene);
                
                glm::vec2 direction = glm::vec2(Input()->ButtonValue(Input()->AIM_X, controllerObjects[i]->playerID), Input()->ButtonValue(Input()->AIM_X, controllerObjects[i]->playerID));
                float directionLength = glm::length(direction);
                if (directionLength < 0.001f)
                    direction = glm::vec2(1.f, 0.f);
                else
                    direction = direction / directionLength;
                
                float bulletSpeed = 10.f;
                GameEntityCreator().CreateBullet(transformComponent->position, bulletSpeed * glm::vec3(direction.x, 0.f, direction.y));
                spawnerComponent->timeSinceSpawn = 0.0f;
            }
        }

        glm::vec3 speedVec = glm::vec3(x * 6000 * deltaTime, 0, z * 6000 * deltaTime);

        Component::Physics* physicsComponent = controllerObjects[i]->entity->GetComponent<Component::Physics>();

        // If there's a physics component attached we use it to move.
        if (physicsComponent != nullptr) {
            if (glm::abs(x) + glm::abs(z) > 0.3f)
                physicsComponent->acceleration = speedVec;
            else
                physicsComponent->acceleration = glm::vec3(0, 0, 0);

        } else if (glm::abs(x) + glm::abs(z) > 0.3f) {
            transformComponent->Move(glm::vec3(x * deltaTime, 0, z * deltaTime));
        }
    }
}
