#include "ControllerSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Physics.hpp>

#include "../Component/Controller.hpp"
#include "../Component/Spawner.hpp"
#include <Util/Log.hpp>
#include "../Util/GameEntityFactory.hpp"
#include "../Util/ControlSchemes.hpp"

using namespace System;

void ControllerSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::Controller*> controllerObjects;
    controllerObjects = scene.GetAll<Component::Controller>();
    
    for (unsigned int i = 0; i < controllerObjects.size(); i++) {
        Component::Transform* transformComponent = controllerObjects[i]->entity->GetComponent<Component::Transform>();
        
        Component::Spawner* spawnerComponent = controllerObjects[i]->entity->GetComponent<Component::Spawner>();
        if (spawnerComponent != nullptr) {
            spawnerComponent->timeSinceSpawn += deltaTime;
            if (Input()->Pressed(controllerObjects[i]->playerID, InputHandler::SHOOT) && spawnerComponent->timeSinceSpawn >= spawnerComponent->delay) {
                Log() << "Shoot\n";
                GameEntityCreator().SetScene(&scene);
                
                glm::vec2 direction = glm::vec2(Input()->ButtonValue(controllerObjects[i]->playerID, Input()->AIM_X), Input()->ButtonValue(controllerObjects[i]->playerID, Input()->AIM_Z));
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
    }
    
    for (unsigned int i = 0; i < controllerObjects.size(); i++)
        controllerObjects[i]->ControlScheme(controllerObjects[i], deltaTime);
}
