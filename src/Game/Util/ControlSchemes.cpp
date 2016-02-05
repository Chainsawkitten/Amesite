#include "ControlSchemes.hpp"

#include <Util/Input.hpp>

#include <Entity/Entity.hpp>
#include <Component/Physics.hpp>

#include <../Game/Component/Controller.hpp>
#include <Component/Transform.hpp>
#include "../Component/Spawner.hpp"
#include "../Util/GameEntityFactory.hpp"

void ControlScheme::Move(Component::Controller* controller, float deltaTime) {
    // Move the player
    float x = Input()->ButtonValue(controller->playerID, InputHandler::MOVE_X);
    float z = Input()->ButtonValue(controller->playerID, InputHandler::MOVE_Z);
    
    if (fabs(x) < 0.001f && fabs(z) < 0.001f) {
        x = Input()->Pressed(controller->playerID, InputHandler::RIGHT) - Input()->Pressed(controller->playerID, InputHandler::LEFT);
        z = Input()->Pressed(controller->playerID, InputHandler::DOWN) - Input()->Pressed(controller->playerID, InputHandler::UP);
    }

    glm::vec3 speedVec = glm::vec3(x * 6000 * deltaTime, 0, z * 6000 * deltaTime);

    Component::Physics* physicsComponent = controller->entity->GetComponent<Component::Physics>();

    // If there's a physics component attached we use it to move.
    if (physicsComponent != nullptr) {
        if (glm::abs(x) + glm::abs(z) > 0.3f)
            physicsComponent->acceleration = speedVec;
        else
            physicsComponent->acceleration = glm::vec3(0, 0, 0);
    } else if (glm::abs(x) + glm::abs(z) > 0.3f) {
        controller->entity->GetComponent<Component::Transform>()->Move(glm::vec3(x * deltaTime, 0, z * deltaTime));
    }
}

void ControlScheme::StickRotate(Component::Controller* controller, float deltaTime) {
    Entity* entity = controller->entity;

    float a = Input()->ButtonValue(controller->playerID, InputHandler::AIM_Z);
    float b = Input()->ButtonValue(controller->playerID, InputHandler::AIM_X);

    if (glm::abs(a) + glm::abs(b) > 0.3f && glm::abs(a) > 0) {
        if(a >= 0)
            entity->GetComponent<Component::Transform>()->yaw = glm::degrees(glm::atan(b / a));
        else
            entity->GetComponent<Component::Transform>()->yaw = 180 + glm::degrees(glm::atan(b / a));
    }
}

void ControlScheme::ArrowKeyRotate(Component::Controller* controller, float deltaTime) {
    Entity* entity = controller->entity;

    bool up = Input()->Pressed(controller->playerID, InputHandler::UP);
    bool down = Input()->Pressed(controller->playerID, InputHandler::DOWN);
    bool right = Input()->Pressed(controller->playerID, InputHandler::RIGHT);
    bool left = Input()->Pressed(controller->playerID, InputHandler::LEFT);

    if (up) {
        if (up && right) {
            entity->GetComponent<Component::Transform>()->yaw = 180 - 45.f;
        } else if (up && left) {
            entity->GetComponent<Component::Transform>()->yaw = 180 + 45.f;
        } else {
            entity->GetComponent<Component::Transform>()->yaw = 180;
        }
    }
    
    if (down) {
        if (down && right) {
            entity->GetComponent<Component::Transform>()->yaw = 45.f;
        } else if (down && left) {
            entity->GetComponent<Component::Transform>()->yaw = -45.f;
        } else {
            entity->GetComponent<Component::Transform>()->yaw = 0;
        }
    }
    
    if (right) {
        if (right && up) {
            entity->GetComponent<Component::Transform>()->yaw = 90 + 45.f;
        } else if (right && down) {
            entity->GetComponent<Component::Transform>()->yaw = 90 - 45.f;
        } else {
            entity->GetComponent<Component::Transform>()->yaw = 90.f;
        }
    }
    
    if (left) {
        if (left && up) {
            entity->GetComponent<Component::Transform>()->yaw = 270 - 45.f;
        } else if (left && down) {
            entity->GetComponent<Component::Transform>()->yaw = 270 + 45.f;
        } else {
            entity->GetComponent<Component::Transform>()->yaw = 270.f;
        }
    }
}

void ControlScheme::ButtonShoot(Component::Controller* controller, float deltaTime) {
    Component::Transform* transformComponent = controller->entity->GetComponent<Component::Transform>();
    
    Component::Spawner* spawnerComponent = controller->entity->GetComponent<Component::Spawner>();
    if (spawnerComponent != nullptr) {
        spawnerComponent->timeSinceSpawn += deltaTime;
        if (Input()->Pressed(controller->playerID, InputHandler::SHOOT) && spawnerComponent->timeSinceSpawn >= spawnerComponent->delay) {
            glm::vec2 direction = glm::vec2(Input()->ButtonValue(controller->playerID, Input()->AIM_X), Input()->ButtonValue(controller->playerID, Input()->AIM_Z));
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

void ControlScheme::AlwaysShoot(Component::Controller* controller, float deltaTime) {
    Component::Transform* transformComponent = controller->entity->GetComponent<Component::Transform>();
    
    Component::Spawner* spawnerComponent = controller->entity->GetComponent<Component::Spawner>();
    if (spawnerComponent != nullptr) {
        spawnerComponent->timeSinceSpawn += deltaTime;
        if (spawnerComponent->timeSinceSpawn >= spawnerComponent->delay) {
            glm::vec2 direction = glm::vec2(1.f, 0.f);
            
            float bulletSpeed = 10.f;
            GameEntityCreator().CreateBullet(transformComponent->position, bulletSpeed * glm::vec3(direction.x, 0.f, direction.y));
            spawnerComponent->timeSinceSpawn = 0.0f;
        }
    }
}
