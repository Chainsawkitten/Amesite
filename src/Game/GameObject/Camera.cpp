#include "Camera.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Lens.hpp>
#include <Engine/Component/Listener.hpp>

#include "../GameObject/Player/SuperPlayer.hpp"
#include "../GameObject/Camera.hpp"

using namespace GameObject;

Camera::Camera(Scene* scene) : SuperGameObject(scene) {
    body = CreateEntity();
    body->AddComponent<Component::Transform>();
    body->AddComponent<Component::Physics>();
    body->AddComponent<Component::Lens>();
    body->AddComponent<Component::Listener>();
}

Camera::~Camera() {
}

void Camera::UpdateRelativePosition(const std::vector<GameObject::SuperPlayer*>& players) {
    int numberOfPlayers = players.size();
    glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 min = glm::vec3(std::numeric_limits<float>::max(), 0.f, std::numeric_limits<float>::max());
    glm::vec3 max = glm::vec3(-std::numeric_limits<float>::max(), 0.f, -std::numeric_limits<float>::max());

    float heightFactor = 1.25f;
    float widthFactor = 0.75f;
    
    for (int i = 0; i < numberOfPlayers; i++) {
        glm::vec3 playerPos = players[i]->GetPosition();
        
        // Find min/max player positions
        if (playerPos.x*widthFactor > max.x)
            max.x = playerPos.x*widthFactor;
        if (playerPos.x*widthFactor < min.x)
            min.x = playerPos.x*widthFactor;
        
        if ((playerPos.z*heightFactor) > max.z)
            max.z = playerPos.z*heightFactor;
        if ((playerPos.z*heightFactor) < min.z)
            min.z = playerPos.z*heightFactor;
        
        cameraPos.x += playerPos.x;
        cameraPos.z += playerPos.z;
    }
    float playerFactor = 1.f / static_cast<float>(numberOfPlayers);
    
    cameraPos.x *= playerFactor;
    cameraPos.z *= playerFactor;
    
    // Calculate how far away the camera should be.
    float distance = glm::distance(min, max) * 1.20f;

    distance = glm::clamp(distance, 80.f, 140.f);
    
    Component::Transform* transform = body->GetComponent<Component::Transform>();
    
    // Transpose camera back the distance.
    const glm::mat4& viewMatrix = transform->modelMatrix;
    glm::vec3 direction = glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);
    cameraPos += direction * distance;

    body->GetComponent<Component::Transform>()->position = cameraPos;
    transform->UpdateModelMatrix();
}
