#include "Camera.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include <Component/Transform.hpp>
#include <Component/Physics.hpp>
#include <Component/Lens.hpp>
#include <Component/Listener.hpp>

#include "../GameObject/Player/SuperPlayer.hpp"
#include "../GameObject/Boss/SuperBoss.hpp"
#include "../GameObject/Camera.hpp"

#include "../Util/Hub.hpp"

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

void Camera::UpdateRelativePosition(const std::vector<GameObject::SuperBoss*>& bosses, float deltaTime) {
    glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 playerCameraPos = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 min = glm::vec3(std::numeric_limits<float>::max(), 0.f, std::numeric_limits<float>::max());
    glm::vec3 max = glm::vec3(-std::numeric_limits<float>::max(), 0.f, -std::numeric_limits<float>::max());

    float heightFactor = 1.55f;
    float widthFactor = 0.75f;
    
    int nrOfPlayers = HubInstance().mPlayers.size();
    for (int i = 0; i < nrOfPlayers; i++) {
        glm::vec3 playerPos = HubInstance().mPlayers[i]->GetPosition();
        
        // Find min/max player positions
        if (playerPos.x*widthFactor > max.x)
            max.x = playerPos.x*widthFactor;
        if (playerPos.x*widthFactor < min.x)
            min.x = playerPos.x*widthFactor;
        
        if ((playerPos.z*heightFactor) > max.z)
            max.z = playerPos.z*heightFactor;
        if ((playerPos.z*heightFactor) < min.z)
            min.z = playerPos.z*heightFactor;
        
        playerCameraPos.x += playerPos.x;
        playerCameraPos.z += playerPos.z;
    }

    cameraPos = playerCameraPos;

    float playerFactor = 1.f / static_cast<float>(nrOfPlayers);

    playerCameraPos.x *= playerFactor;
    playerCameraPos.z *= playerFactor;

    glm::vec3 bossCameraPos = glm::vec3(0.f, 0.f, 0.f);
    int bossFactor = 0;
    int nrOfBosses = bosses.size();
    for (int i = 0; i < nrOfBosses; i++) {
        if (bosses[i] != nullptr) {
            glm::vec3 bossPos = bosses[i]->GetPosition();

            if (glm::distance(playerCameraPos, bossPos) < 100.f) {
                // Find min/max player positions
                if (bossPos.x*widthFactor > max.x)
                    max.x = bossPos.x*widthFactor;
                if (bossPos.x*widthFactor < min.x)
                    min.x = bossPos.x*widthFactor;

                if ((bossPos.z*heightFactor) > max.z)
                    max.z = bossPos.z*heightFactor;
                if ((bossPos.z*heightFactor) < min.z)
                    min.z = bossPos.z*heightFactor;

                bossCameraPos.x += bossPos.x;
                bossCameraPos.z += bossPos.z;
                bossFactor++;
                cameraPos += bossCameraPos;
            }
        }
    }

    float cameraFactor = 1.f / static_cast<float>(nrOfPlayers + bossFactor);

    cameraPos.x *= cameraFactor;
    cameraPos.z *= cameraFactor;

    // Calculate how far away the camera should be.
    float distance = glm::distance(min, max) * 1.20f;

    distance = glm::clamp(distance, 100.f, 120.f);
    
    Component::Transform* transform = body->GetComponent<Component::Transform>();
    
    // Transpose camera back the distance.
    const glm::mat4& viewMatrix = transform->modelMatrix;
    glm::vec3 direction = glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);
    cameraPos += direction * distance;
    glm::vec3 velocity = (cameraPos - transform->position) * 2.f;
    transform->position += velocity * deltaTime;
    transform->UpdateModelMatrix();
}
