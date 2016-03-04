#include "ControlSchemes.hpp"

#include <Util/Input.hpp>

#include <Entity/Entity.hpp>
#include <Component/Physics.hpp>

#include <../Game/Component/Controller.hpp>
#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <Component/PointLight.hpp>
#include "../Component/Spawner.hpp"
#include "../Component/Damage.hpp"
#include "../Component/Health.hpp"
#include <Component/SoundSource.hpp>
#include "../GameObject/Bullet.hpp"
#include "../Util/GameEntityFactory.hpp"
#include <Util/Picking.hpp>
#include "../Util/MainCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <MainWindow.hpp>
#include <Util/Log.hpp>

#include <Game/GameObject/Bullet.hpp>

void ControlScheme::Empty(Component::Controller* controller, float deltaTime) {}

void ControlScheme::Move(Component::Controller* controller, float deltaTime) {
    
    // Move the player
    double x = Input()->ButtonValue(controller->playerID, InputHandler::MOVE_X);
    double z = Input()->ButtonValue(controller->playerID, InputHandler::MOVE_Z);
    glm::vec2 direction = glm::vec2(x, z);
    
    if (glm::length(direction)<Input()->MoveDeadzone()) {
        x = Input()->ButtonValue(controller->playerID, InputHandler::RIGHT) - Input()->ButtonValue(controller->playerID, InputHandler::LEFT);
        z = Input()->ButtonValue(controller->playerID, InputHandler::DOWN) - Input()->ButtonValue(controller->playerID, InputHandler::UP);
        direction = glm::vec2(x, z);
    }
    
    glm::vec3 speedVec = glm::vec3(x * controller->speed * deltaTime, 0, z * controller->speed * deltaTime);
    
    Component::Physics* physicsComponent = controller->entity->GetComponent<Component::Physics>();
    
    // If there's a physics component attached we use it to move.
    if (physicsComponent != nullptr) {
        if (glm::length(direction)>Input()->MoveDeadzone())
            physicsComponent->acceleration = speedVec;
        else
            physicsComponent->acceleration = glm::vec3(0, 0, 0);
    } else if (glm::length(direction)>Input()->MoveDeadzone()) {
        controller->entity->GetComponent<Component::Transform>()->Move(glm::vec3(x * deltaTime * controller->speed, 0, z * deltaTime));
    }
}

void ControlScheme::StickRotate(Component::Controller* controller, float deltaTime) {
    Entity* entity = controller->entity;
    
    double a = Input()->ButtonValue(controller->playerID, InputHandler::AIM_Z);
    double b = Input()->ButtonValue(controller->playerID, InputHandler::AIM_X);
    glm::vec2 direction = glm::vec2(a, b);
    
    if (glm::length(direction) > Input()->AimDeadzone()) {
        Input()->SetLastValidAimDirection(controller->playerID, glm::vec2(b, a));
        if (a >= 0)
            entity->GetComponent<Component::Transform>()->yaw = (float)glm::degrees(glm::atan(b / a));
        else
            entity->GetComponent<Component::Transform>()->yaw = 180.f + (float)glm::degrees(glm::atan(b / a));
    }
}

void ControlScheme::MouseRotate(Component::Controller* controller, float deltaTime) {
    Entity* entity = controller->entity;
    Component::Transform* transformComponent = controller->entity->GetComponent<Component::Transform>();
    
    glm::vec4 playerPosition = transformComponent->modelMatrix*glm::vec4(0.f, 0.f, 0.f, 1.f);
    
    glm::vec2 mouseCoordinates(Input()->CursorX(), Input()->CursorY());
    Entity& mainCamera = MainCameraInstance().GetMainCamera();
    glm::mat4 projectionMatrix = mainCamera.GetComponent<Component::Lens>()->GetProjection(MainWindow::GetInstance()->GetSize());
    glm::mat4 viewMatrix = mainCamera.GetComponent<Component::Transform>()->GetWorldOrientation()*glm::translate(glm::mat4(), -mainCamera.GetComponent<Component::Transform>()->GetWorldPosition());
    
    glm::vec4 worldRay = Picking::CreateWorldRay(mouseCoordinates, viewMatrix, projectionMatrix);
    
    glm::vec4 directionInPlane = Picking::CreatePlayerAimDirection(worldRay, playerPosition, glm::vec4(mainCamera.GetComponent<Component::Transform>()->position, 1.f));
    glm::vec2 direction(directionInPlane.x, directionInPlane.z);
    
    if (direction.y >= 0)
        entity->GetComponent<Component::Transform>()->yaw = +(float)glm::degrees(glm::atan(direction.x / direction.y));
    else
        entity->GetComponent<Component::Transform>()->yaw = 180.f + (float)glm::degrees(glm::atan(direction.x / direction.y));
}

void ControlScheme::ArrowKeysMove(Component::Controller* controller, float deltaTime) {
    Entity* entity = controller->entity;
    
    // Move the player
    bool up = Input()->Pressed(controller->playerID, InputHandler::UP);
    bool down = Input()->Pressed(controller->playerID, InputHandler::DOWN);
    bool right = Input()->Pressed(controller->playerID, InputHandler::RIGHT);
    bool left = Input()->Pressed(controller->playerID, InputHandler::LEFT);
    
    glm::vec3 speedVec = glm::vec3((right - left) * controller->speed * deltaTime, 0.f, (down - up) * controller->speed * deltaTime);
    
    Component::Physics* physicsComponent = entity->GetComponent<Component::Physics>();
    
    // If there's a physics component attached we use it to move.
    if (physicsComponent != nullptr)
        physicsComponent->acceleration = speedVec;
    else
        controller->entity->GetComponent<Component::Transform>()->Move(speedVec);
    
}

void ControlScheme::AlwaysShoot(Component::Controller* controller, float deltaTime) {
    Component::Transform* transformComponent = controller->entity->GetComponent<Component::Transform>();
    Component::Spawner* spawnerComponent = controller->entity->GetComponent<Component::Spawner>();
    if (spawnerComponent != nullptr) {
        if (spawnerComponent->timeSinceSpawn >= spawnerComponent->delay) {
            glm::vec2 direction = glm::vec2(1 - ((rand() % 1000) / 1000.f) * 2, 1 - ((rand() % 1000) / 1000.f) * 2);
            
            float bulletSpeed = 20.f;
            GameEntityCreator().CreateEnemyBullet(transformComponent->GetWorldPosition(), bulletSpeed * glm::normalize(glm::vec3(direction.x, 0.f, direction.y)), spawnerComponent->faction);
            spawnerComponent->timeSinceSpawn = 0.0f;
        }
    }
}

void ControlScheme::AutoRotate(Component::Controller* controller, float deltaTime) {
    Entity* entity = controller->entity;
    
    Component::Physics* physicsComponent = entity->GetComponent<Component::Physics>();
    
    // If there's a physics component attached we use it to move.
    if (physicsComponent != nullptr)
        physicsComponent->angularVelocity = glm::vec3(0, controller->speed, 0);
    else
        entity->GetComponent<Component::Transform>()->yaw += controller->speed;
    
}

void ControlScheme::RandomMove(Component::Controller* controller, float deltaTime) {
    
    // Move the player
    float x = 1 - ((rand() % 1000 + 1) / 1000.f) * 2;
    float z = 1 - ((rand() % 1000 + 1) / 1000.f) * 2;
    
    glm::vec3 speedVec = glm::vec3(x * controller->speed * deltaTime, 0, z * controller->speed * deltaTime);
    
    Component::Physics* physicsComponent = controller->entity->GetComponent<Component::Physics>();
    
    // If there's a physics component attached we use it to move.
    if (physicsComponent != nullptr) {
        if (glm::abs(x) + glm::abs(z) > 0.3f)
            physicsComponent->acceleration = speedVec;
        else
            physicsComponent->acceleration = glm::vec3(0, 0, 0);
    }
    else if (glm::abs(x) + glm::abs(z) > 0.3f) {
        controller->entity->GetComponent<Component::Transform>()->Move(glm::vec3(x * deltaTime * controller->speed, 0, z * deltaTime * controller->speed));
    }
}

void ControlScheme::Aim(Component::Controller* controller, float deltaTime) {

    Entity* entity = controller->entity;

    // Move the player
    float x = Input()->ButtonValue(controller->playerID, InputHandler::AIM_X);
    float z = Input()->ButtonValue(controller->playerID, InputHandler::AIM_Z);
    
    glm::vec3 movement = glm::vec3(x, 0, z);
    
    if (glm::length(movement) > Input()->AimDeadzone()) {
        
        Component::Transform* transform = entity->GetComponent<Component::Transform>();

        float oldAngle = glm::radians(transform->GetWorldYawPitchRoll().x);

        glm::vec3 oldPoint = transform->position + glm::normalize(glm::vec3(glm::sin(oldAngle), 0, glm::cos(oldAngle))) * 5.f;
        glm::vec3 newPoint = oldPoint - movement;
        glm::vec3 oldDirection = glm::normalize(oldPoint - transform->position);
        glm::vec3 newDirection = glm::normalize(newPoint - transform->position);
        float dot = glm::dot(glm::vec2(oldDirection.x, oldDirection.z), glm::vec2(newDirection.x, newDirection.z));
        
        if (dot > 1.f)
            dot = 1.f;
        else if (dot < -1.f)
            dot = -1.f;
        
        float angle = glm::acos(dot);
        
        if (glm::cross(oldDirection, newDirection).y > 0)
            angle *= -1;
        
        transform->yaw = glm::degrees(oldAngle + angle);
        
    }
    
}

void ControlScheme::MouseAim(Component::Controller* controller, float deltaTime) {
    
    Entity* entity = controller->entity;
    
    // Move the player
    float x = Input()->DeltaCursorX();
    float z = Input()->DeltaCursorY();
    
    if (x != 0 || z != 0) {
        
        glm::vec3 movement = glm::normalize(glm::vec3(x, 0, z)) * 2.f;
        
        Component::Transform* transform = entity->GetComponent<Component::Transform>();
        float oldAngle = glm::radians(transform->yaw);
        
        glm::vec3 oldPoint = transform->position + glm::normalize(glm::vec3(glm::sin(oldAngle), 0, glm::cos(oldAngle))) * 5.f;
        glm::vec3 newPoint = oldPoint - movement;
        glm::vec3 oldDirection = glm::normalize(oldPoint - transform->position);
        glm::vec3 newDirection = glm::normalize(newPoint - transform->position);
        float dot = glm::dot(glm::vec2(oldDirection.x, oldDirection.z), glm::vec2(newDirection.x, newDirection.z));
        
        if (dot > 1.f)
            dot = 1.f;
        else if (dot < -1.f)
            dot = -1.f;
        
        float angle = glm::acos(dot);
        
        if (glm::cross(oldDirection, newDirection).y > 0)
            angle *= -1;
        
        transform->yaw = glm::degrees(oldAngle + angle);
        
    }
    
}

void ControlScheme::AimedFire(Component::Controller* controller, float deltaTime) {

    Component::Spawner* spawnerComponent = controller->entity->GetComponent<Component::Spawner>();
    
    if (spawnerComponent != nullptr) {
        if (Input()->Pressed(controller->playerID, InputHandler::SHOOT) && spawnerComponent->timeSinceSpawn >= spawnerComponent->delay) {
            // Spawn bullet.
            Entity* entity = controller->entity;
            
            Component::Transform* transform = entity->GetComponent<Component::Transform>();

            float angle = glm::radians(transform->GetWorldYawPitchRoll().x);
            
            glm::vec3 direction = glm::normalize(glm::vec3(glm::sin(angle), 0, glm::cos(angle)));

            float bulletSpeed = 60.f;

            GameObject::Bullet* bullet = GameEntityCreator().CreatePlayerBullet(transform->GetWorldPosition(), bulletSpeed *  direction, spawnerComponent->faction);
            //bullet->node->AddComponent<Component::PointLight>()->attenuation = 2.f;
            spawnerComponent->timeSinceSpawn = 0.f;
            
            // Shoot sound.
            Component::SoundSource* soundSource = controller->entity->GetComponent<Component::SoundSource>();
            if (soundSource != nullptr && soundSource->soundBuffer != nullptr) {
                soundSource->Play();
            }
        }
    }
}

void ControlScheme::AutoAimedFire(Component::Controller* controller, float deltaTime) {

    Component::Spawner* spawnerComponent = controller->entity->GetComponent<Component::Spawner>();

    if (spawnerComponent != nullptr) {
        if (spawnerComponent->timeSinceSpawn >= spawnerComponent->delay) {
            Entity* entity = controller->entity;

            Component::Transform* transform = entity->GetComponent<Component::Transform>();
            float angle = glm::radians(transform->yaw);

            glm::vec3 direction = glm::normalize(glm::vec3(glm::sin(angle), 0, glm::cos(angle)));

            float bulletSpeed = 10.f;
            GameObject::Bullet* bullet = GameEntityCreator().CreateEnemyBullet(transform->position, bulletSpeed *  direction, 1);
            spawnerComponent->timeSinceSpawn = 0.0f;
        }
    }
}

void ControlScheme::Boost(Component::Controller* controller, float deltaTime) {

    //Entity* entity = controller->entity;

    //Component::Transform* transform = entity->GetComponent<Component::Transform>();
    //float oldAngle = glm::radians(transform->GetWorldYawPitchRoll().x);

    //if(Input()->Triggered(controller->playerID, InputHandler::BOOST))
    //    entity->GetComponent<Component::Physics>()->acceleration += glm::normalize(glm::vec3(glm::sin(oldAngle), 0, glm::cos(oldAngle))) * 50000.f;
}

void ControlScheme::Shield(Component::Controller* controller, float deltaTime) {
    //if (Input()->Pressed(controller->playerID, InputHandler::SHOOT))
        //GameEntityCreator().CreateShield(controller->entity, glm::vec3(0, 0, 4), 1.f, 100);
}
