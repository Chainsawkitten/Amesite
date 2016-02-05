#include "CameraUpdate.hpp"
#include "../Component/Controller.hpp"
#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <Util/Log.hpp>
#include <Component/Physics.hpp>
#include <limits.h>

void UpdateCamera(Entity* camera, const std::vector<Entity*>& players){
    int numberOfPlayers = players.size();
    glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 min = glm::vec3(std::numeric_limits<float>::max(), 0.f, std::numeric_limits<float>::max());
    glm::vec3 max = glm::vec3(-std::numeric_limits<float>::max(), 0.f, -std::numeric_limits<float>::max());
    
    //Find camera.  
    float heightFactor = 3.f;
    float widthFactor = 2.f;

    for (int i = 0; i < numberOfPlayers; i++) {
        glm::vec3 playerPos = players[i]->GetComponent<Component::Transform>()->position;

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

    camera->GetComponent<Component::Transform>()->position = cameraPos;
}