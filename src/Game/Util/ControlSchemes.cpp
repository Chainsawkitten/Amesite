#include "ControlSchemes.hpp"

#include <Util/Input.hpp>

#include <Entity\Entity.hpp>
#include <Component\Physics.hpp>

#include <../Game/Component/Controller.hpp>

void ControlScheme::StickMove(Component::Controller* controller, float deltaTime) {

    Entity* entity = controller->entity;

    //Move the player
    float x = Input()->ButtonValue(Input()->MOVE_X, controller->playerID);
    float z = Input()->ButtonValue(Input()->MOVE_Z, controller->playerID);

    glm::vec3 speedVec = glm::vec3(x * 6000 * deltaTime, 0, z * 6000 * deltaTime);

    Component::Physics* physicsComponent = controller->entity->GetComponent<Component::Physics>();

    //If there's a physics component attached we use it to move.
    if (physicsComponent != NULL) {

        if (glm::abs(x) + glm::abs(z) > 0.3f)
            physicsComponent->acceleration = speedVec;
        else
            physicsComponent->acceleration = glm::vec3(0, 0, 0);

    }
    else if (glm::abs(x) + glm::abs(z) > 0.3f)
        controller->entity->GetComponent<Component::Transform>()->Move(glm::vec3(x * deltaTime, 0, z * deltaTime));

}

void ControlScheme::StickRotate(Component::Controller* controller, float deltaTime) {

    Entity* entity = controller->entity;

    float a = Input()->ButtonValue(Input()->AIM_X, controller->playerID);
    float b = Input()->ButtonValue(Input()->AIM_Z, controller->playerID);

    if (glm::abs(a) + glm::abs(b) > 0.3f && glm::abs(a) > 0) {

        if(a >= 0)
            entity->GetComponent<Component::Transform>()->yaw = glm::degrees(glm::atan(b / a));
        else
            entity->GetComponent<Component::Transform>()->yaw = 180 + glm::degrees(glm::atan(b / a));
    }

}

void ControlScheme::ArrowKeyRotate(Component::Controller* controller, float deltaTime) {

    Entity* entity = controller->entity;

    float up = Input()->ButtonValue(Input()->UP, controller->playerID);
    float down = Input()->ButtonValue(Input()->DOWN, controller->playerID);
    float right = Input()->ButtonValue(Input()->RIGHT, controller->playerID);
    float left = Input()->ButtonValue(Input()->LEFT, controller->playerID);

    Log() << left;

    if (up) {

        if (up && right) {

            entity->GetComponent<Component::Transform>()->yaw = 180 - 45.f;

        }
        else if (up && left) {

            entity->GetComponent<Component::Transform>()->yaw = 180 + 45.f;

        }
        else {

            entity->GetComponent<Component::Transform>()->yaw = 180;


        }

    }
    if (down) {

        if (down && right) {

            entity->GetComponent<Component::Transform>()->yaw = 45.f;

        }
        else if (down && left) {

            entity->GetComponent<Component::Transform>()->yaw = -45.f;

        }
        else {

            entity->GetComponent<Component::Transform>()->yaw = 0;


        }

    }
    if (right) {

        if (right && up) {

            entity->GetComponent<Component::Transform>()->yaw = 90 + 45.f;

        }
        else if (right && down) {

            entity->GetComponent<Component::Transform>()->yaw = 90 - 45.f;

        }
        else {

            entity->GetComponent<Component::Transform>()->yaw = 90.f;


        }

    }
    if (left) {

        if (left && up) {

            entity->GetComponent<Component::Transform>()->yaw = 270 - 45.f;

        }
        else if (left && down) {

            entity->GetComponent<Component::Transform>()->yaw = 270 + 45.f;

        }
        else {

            entity->GetComponent<Component::Transform>()->yaw = 270.f;


        }

    }

}

void ControlScheme::ArrowKeysMove(Component::Controller* controller, float deltaTime) {

    Entity* entity = controller->entity;

    //Move the player
    float up = Input()->ButtonValue(Input()->UP, controller->playerID);
    float down = Input()->ButtonValue(Input()->DOWN, controller->playerID);
    float right = Input()->ButtonValue(Input()->RIGHT, controller->playerID);
    float left = Input()->ButtonValue(Input()->LEFT, controller->playerID);

    glm::vec3 speedVec = glm::vec3((right - left) * 6000 * deltaTime, 0, (down - up) * 6000 * deltaTime);

    Component::Physics* physicsComponent = entity->GetComponent<Component::Physics>();

    //If there's a physics component attached we use it to move.
    if (physicsComponent != NULL) {

        if (glm::abs(up) + glm::abs(down) + glm::abs(right) + glm::abs(left) > 0.3f)
            physicsComponent->acceleration = speedVec;
        else
            physicsComponent->acceleration = glm::vec3(0, 0, 0);

    }
    else if (glm::abs(up) + glm::abs(down) > 0.3f)
        controller->entity->GetComponent<Component::Transform>()->Move((right - left) * 6000 * deltaTime, 0, (down - up) * deltaTime * 20);

}
