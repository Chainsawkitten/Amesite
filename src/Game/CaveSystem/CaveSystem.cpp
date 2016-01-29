#include "CaveSystem.hpp"
#include <Geometry/Cube.hpp>
#include <Resources.hpp>
#include <Component/Mesh.hpp>
#include <Component/Transform.hpp>

using namespace Caves;

CaveSystem::CaveSystem() {
    mScene = nullptr;
}

CaveSystem::CaveSystem(Scene* scene) {
    mScene = scene;
}

CaveSystem::~CaveSystem() {
    //TODO: DELETE EVERYTHING
}

void CaveSystem::GenerateCaveSystem() {
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 25; j++) {
            if(mMap[i][j] > 0.f){
                Entity* wall = mScene->CreateEntity();
                wall->AddComponent<Component::Transform>();
                wall->AddComponent<Component::Mesh>();
                wall->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
                wall->GetComponent<Component::Transform>()->position = glm::vec3(float(j), -float(i), 0.f);
                walls.push_back(wall);
            }
        }
    }
}