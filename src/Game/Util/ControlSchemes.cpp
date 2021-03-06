#include "ControlSchemes.hpp"

#include <Engine/Util/Input.hpp>

#include <Engine/Entity/Entity.hpp>
#include <Engine/Component/Physics.hpp>

#include "../Component/Controller.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Lens.hpp>
#include <Engine/Component/PointLight.hpp>
#include "../Component/Spawner.hpp"
#include "../Component/Damage.hpp"
#include "../Component/Health.hpp"
#include <Engine/Component/SoundSource.hpp>
#include "../GameObject/Bullet.hpp"
#include "../Util/GameEntityFactory.hpp"
#include <Engine/Util/Picking.hpp>
#include "../Util/Hub.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <Engine/MainWindow.hpp>
#include <Engine/Util/Log.hpp>
#include <random>

#include "../Game/GameObject/Bullet.hpp"
#include "../Game/GameObject/Camera.hpp"
#include "../Game/GameObject/Player/Player1.hpp"
#include "../Game/GameObject/Player/Player2.hpp"

void ControlScheme::Empty(Component::Controller* controller, float deltaTime) {}

void ControlScheme::Move(Component::Controller* controller, float deltaTime) {

    // Move the player
    double x = Input()->ButtonValue(controller->playerID, InputHandler::MOVE_X);
    double z = Input()->ButtonValue(controller->playerID, InputHandler::MOVE_Z);

    glm::vec2 direction = glm::vec2(x, z);
    
    if (glm::length(direction) < Input()->MoveDeadzone()) {
        x = Input()->ButtonValue(controller->playerID, InputHandler::RIGHT) - Input()->ButtonValue(controller->playerID, InputHandler::LEFT);
        z = Input()->ButtonValue(controller->playerID, InputHandler::DOWN) - Input()->ButtonValue(controller->playerID, InputHandler::UP);
        direction = glm::vec2(x, z);
    }
    
    if (controller->device == InputHandler::JOYSTICK)
        z = -z;

    glm::vec3 speedVec = glm::vec3(x * controller->speed, 0, z * controller->speed);
    
    Component::Physics* physicsComponent = controller->entity->GetComponent<Component::Physics>();
    
    // If there's a physics component attached we use it to move.
    if (physicsComponent != nullptr) {
        if (glm::length(direction)>Input()->MoveDeadzone())
            physicsComponent->acceleration = speedVec;
        else
            physicsComponent->acceleration = glm::vec3(0, 0, 0);
    } else if (glm::length(direction)>Input()->MoveDeadzone()) {
        controller->entity->GetComponent<Component::Transform>()->Move(glm::vec3(x * deltaTime * controller->speed, 0, z * deltaTime * controller->speed));
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

    GameObject::Camera& mainCamera = HubInstance().GetMainCamera();
    Component::Transform* cameraTransform = mainCamera.body->GetComponent<Component::Transform>();
    glm::mat4 projectionMatrix = mainCamera.body->GetComponent<Component::Lens>()->GetProjection(MainWindow::GetInstance()->GetSize());
    glm::mat4 viewMatrix = cameraTransform->GetWorldOrientation()*glm::translate(glm::mat4(), -cameraTransform->GetWorldPosition());
    
    glm::vec4 worldRay = Picking::CreateWorldRay(mouseCoordinates, viewMatrix, projectionMatrix);
    
    glm::vec4 directionInPlane = Picking::CreatePlayerAimDirection(worldRay, playerPosition, glm::vec4(cameraTransform->position, 1.f));
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

void ControlScheme::AlwaysShootClosestPlayer(Component::Controller* controller, float deltaTime) {
    Component::Transform* transformComponent = controller->entity->GetComponent<Component::Transform>();
    Component::Spawner* spawnerComponent = controller->entity->GetComponent<Component::Spawner>();
    if (spawnerComponent != nullptr) {
        if (spawnerComponent->timeSinceSpawn >= spawnerComponent->delay) {
            spawnerComponent->timeSinceSpawn = 0.0f;
            
            float minimumDistance = std::numeric_limits<float>().max();
            glm::vec3 targetPlayerPosition;

            glm::vec3 transformWorldPosition = transformComponent->GetWorldPosition();
            for (auto& player : HubInstance().mPlayers) {
                if (player->Active()) {
                    float distanceToPlayer = glm::distance(player->GetPosition(), transformWorldPosition);
                    if (distanceToPlayer < minimumDistance) {
                        minimumDistance = distanceToPlayer;
                        targetPlayerPosition = player->GetPosition();
                    }
                }
            }

            glm::vec3 targetDirection = targetPlayerPosition - transformWorldPosition;
            if (glm::length(targetDirection) > 0.001f)
                GameEntityCreator().CreateEnemyBullet(transformComponent->GetWorldPosition(), 20.f * glm::normalize(glm::vec3(targetDirection.x, 0.f, targetDirection.z)), spawnerComponent->faction);
        }
    }
}

void ControlScheme::AlwaysShootRandomPlayer(Component::Controller* controller, float deltaTime) {
    Component::Transform* transformComponent = controller->entity->GetComponent<Component::Transform>();
    Component::Spawner* spawnerComponent = controller->entity->GetComponent<Component::Spawner>();
    if (spawnerComponent != nullptr) {
        if (spawnerComponent->timeSinceSpawn >= spawnerComponent->delay) {
            spawnerComponent->timeSinceSpawn = 0.0f;
            std::default_random_engine rng(time(0));
            std::uniform_int_distribution<unsigned int> playerDistribution(0, HubInstance().mPlayers.size() - 1);
            glm::vec3 targetPlayerPosition = HubInstance().mPlayers[playerDistribution(rng)]->GetPosition();
            glm::vec3 transformWorldPosition = transformComponent->GetWorldPosition();
            glm::vec3 targetDirection = targetPlayerPosition - transformWorldPosition;
            if (glm::length(targetDirection) > 0.001f)
                GameEntityCreator().CreateEnemyBullet(transformComponent->GetWorldPosition(), 20.f * glm::normalize(glm::vec3(targetDirection.x, 0.f, targetDirection.z)), spawnerComponent->faction);

        }
    }
}

void ControlScheme::LookAtClosestPlayer(Component::Controller* controller, float deltaTime) {
    Component::Transform* transformComponent = controller->entity->GetComponent<Component::Transform>();
    float minimumDistance = std::numeric_limits<float>().max();
    glm::vec3 targetPlayerPosition;

    glm::vec3 transformWorldPosition = transformComponent->GetWorldPosition();
    for (auto& player : HubInstance().mPlayers) {
        if (player->Active()) {
            float distanceToPlayer = glm::distance(player->GetPosition(), transformWorldPosition);
            if (distanceToPlayer < minimumDistance) {
                minimumDistance = distanceToPlayer;
                targetPlayerPosition = player->GetPosition();
            }
        }
    }

    glm::vec3 targetDirection = targetPlayerPosition - transformWorldPosition;
    if (glm::length(targetDirection) > 0.001f) {
        float angle = glm::degrees(glm::acos(glm::dot(glm::normalize(targetDirection), glm::vec3(0.f, 0.f, 1.f))));
        if (targetDirection.x > 0.f)
            transformComponent->yaw = angle;
        else
            transformComponent->yaw = 360.f - angle;
    }
}

void ControlScheme::AccelerateTowardsClosestPlayer(Component::Controller* controller, float deltaTime) {
    Component::Transform* transformComponent = controller->entity->GetComponent<Component::Transform>();
    float minimumDistance = std::numeric_limits<float>().max();
    glm::vec3 targetPlayerPosition;

    glm::vec3 transformWorldPosition = transformComponent->GetWorldPosition();
    for (auto& player : HubInstance().mPlayers) {
        if (player->Active()) {
            float distanceToPlayer = glm::distance(player->GetPosition(), transformWorldPosition);
            if (distanceToPlayer < minimumDistance) {
                minimumDistance = distanceToPlayer;
                targetPlayerPosition = player->GetPosition();
            }
        }
    }

    glm::vec3 targetDirection = targetPlayerPosition - transformWorldPosition;

    if (glm::length(targetDirection) > 0.001f) {
        Component::Physics* physics = controller->entity->GetComponent<Component::Physics>();
        if (physics != nullptr) {
            if (glm::length(physics->acceleration) < 0.01f) {
                physics->acceleration = (targetDirection * controller->speed);
            } else {
                float accelerationFactor = ((glm::dot(glm::normalize(targetDirection), glm::normalize(physics->acceleration)) - 1.f) * -1.f) * 20.f + 1.f;
                physics->acceleration += targetDirection * controller->speed * accelerationFactor * accelerationFactor * accelerationFactor;
            }
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
    float z = -Input()->ButtonValue(controller->playerID, InputHandler::AIM_Z);
    
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

            GameEntityCreator().CreatePlayerBullet(transform->GetWorldPosition(), bulletSpeed *  direction, spawnerComponent->faction);
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
            GameEntityCreator().CreateEnemyBullet(transform->position, bulletSpeed *  direction, 1);
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
