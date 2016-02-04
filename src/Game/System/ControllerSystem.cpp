#include "ControllerSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Physics.hpp>

#include "../Component/Controller.hpp"

using namespace System;

ControllerSystem::ControllerSystem() {



}

ControllerSystem::~ControllerSystem() {



}

void ControllerSystem::Update(Scene& scene, float deltaTime) {

    Component::Controller* a;

    std::vector<Component::Controller*> controllerObjects;
    controllerObjects = scene.GetAll<Component::Controller>();

    for (unsigned int i = 0; i < controllerObjects.size(); i++) {

        //Move the player
        float x = Input()->ButtonValue(Input()->MOVE_X, controllerObjects[i]->playerID);
        float z = Input()->ButtonValue(Input()->MOVE_Z, controllerObjects[i]->playerID);

        glm::vec3 speedVec = glm::vec3(x * 6000 * deltaTime, 0, z * 6000 * deltaTime);

        Component::Physics* physicsComponent = controllerObjects[i]->entity->GetComponent<Component::Physics>();

        //If there's a physics component attached we use it to move.
        if (physicsComponent != NULL) {

            if (glm::abs(x) + glm::abs(z) > 0.3f)
                physicsComponent->acceleration = speedVec;
            else
                physicsComponent->acceleration = glm::vec3(0, 0, 0);

        }
        else if (glm::abs(x) + glm::abs(z) > 0.3f)
            controllerObjects[i]->entity->GetComponent<Component::Transform>()->Move(glm::vec3(x * deltaTime, 0, z * deltaTime));
    
    }

}
