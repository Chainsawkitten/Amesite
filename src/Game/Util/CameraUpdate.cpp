#include "CameraUpdate.hpp"
#include "../Component/Controller.hpp"
#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <Util/Log.hpp>
#include <Component/Physics.hpp>

void UpdateCamera(Entity* camera, const std::vector<Entity*>& players){
    int numberOfPlayers = players.size();
    float cameraX, cameraZ, cameraY;
    glm::vec2 min, max;
    cameraX = cameraZ = cameraY = 0.f;
    //Find camera.
    
    float heightFactor = 3.f;
    float widthFactor = 2.f;

    for (int i = 0; i < numberOfPlayers; i++) {
        float x, z;
        x = players[i]->GetComponent<Component::Transform>()->position.x;
        z = players[i]->GetComponent<Component::Transform>()->position.z;

        //Find min/max player positions
        if (x*widthFactor > max.x)
            max.x = x*widthFactor;
        else if (x*widthFactor < min.x)
            min.x = x*widthFactor;

        if ((z*heightFactor) > max.y)
            max.y = z*heightFactor;
        else if ((z*heightFactor) < min.y)
            min.y = z*heightFactor;

        cameraX += x;
        cameraZ += z;
    }
    float playerFactor = 1.f / static_cast<float>(numberOfPlayers);

    cameraX = cameraX * playerFactor;
    cameraZ = cameraZ * playerFactor;
    float distance = glm::distance(min, max)*0.75f;
    distance = glm::clamp(distance, 30.f, 60.f);
    cameraY = distance;

    glm::vec3 endPosition = glm::vec3(cameraX,cameraY,cameraZ);

    camera->GetComponent<Component::Transform>()->position = glm::vec3(cameraX, cameraY, cameraZ);
}