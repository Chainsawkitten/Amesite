#include "Camera.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Lens.hpp>
#include <Engine/Component/Listener.hpp>

#include "../GameObject/Player.hpp"
#include "../GameObject/Camera.hpp"

using namespace GameObject;

Camera::Camera(Scene* scene) : SuperGameObject(scene) {
    body = CreateEntity(scene);
    body->AddComponent<Component::Transform>();
    body->AddComponent<Component::Physics>();
    body->AddComponent<Component::Lens>();
    body->AddComponent<Component::Listener>();
}

Camera::~Camera() {
}

void Camera::UpdateRelativePosition(const std::vector<GameObject::Player*>& players) {
    int numberOfPlayers = players.size();
    glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 min = glm::vec3(std::numeric_limits<float>::max(), 0.f, std::numeric_limits<float>::max());
    glm::vec3 max = glm::vec3(-std::numeric_limits<float>::max(), 0.f, -std::numeric_limits<float>::max());

    //Find camera.  
    float heightFactor = 3.f;
    float widthFactor = 2.f;

    for (int i = 0; i < numberOfPlayers; i++) {
        glm::vec3 playerPos = players[i]->GetPosition();

        //Find min/max player positions
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
    float distance = glm::distance(min, max)*0.75f;
    distance = glm::clamp(distance, 30.f, 60.f);
    cameraPos.y = distance;

    body->GetComponent<Component::Transform>()->position = cameraPos;
}
