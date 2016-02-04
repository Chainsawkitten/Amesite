#include "CameraSystem.hpp"
#include "../Component/Controller.hpp"
#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/Lens.hpp>
#include <Util/Log.hpp>

System::CameraSystem::CameraSystem(){
}

System::CameraSystem::~CameraSystem(){
}

void System::CameraSystem::Update(Scene & scene, float deltaTime){
    int numberOfPlayers = scene.GetAll<Component::Controller>().size();
    int numberOfLenses = scene.GetAll<Component::Lens>().size();
    float cameraX, cameraZ, cameraY;
    float minX, minZ, maxX, maxZ;
    minX = minZ = maxX = maxZ = cameraX = cameraZ = cameraY = 0.f;
    //Find camera.
    Entity* camera;
    for (unsigned int i = 0; i < numberOfLenses; i++) {
        if (scene.GetAll<Component::Lens>()[i]->entity->GetComponent<Component::Transform>() != nullptr)
            camera = scene.GetAll<Component::Lens>()[i]->entity;
    }

    for (int i = 0; i < numberOfPlayers; i++){
        if (scene.GetAll<Component::Controller>()[i]->entity->GetComponent<Component::Transform>() != nullptr) {
            float x, z;
            x = scene.GetAll<Component::Controller>()[i]->entity->GetComponent<Component::Transform>()->position.x;
            z = scene.GetAll<Component::Controller>()[i]->entity->GetComponent<Component::Transform>()->position.z;
            if()
            cameraX += x;
            cameraZ += z;
        }
        cameraX = cameraX * 0.5f;
        cameraZ = cameraZ * 0.5f;
        cameraY = 20.f;
        camera->GetComponent<Component::Transform>()->position = glm::vec3(cameraX, cameraY, cameraZ);
    }
}
