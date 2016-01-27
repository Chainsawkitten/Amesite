#include "CaveSystem.hpp"
#include "../Geometry/Cube.hpp"
#include "../Resources.hpp"

using namespace Caves;

void CaveSystem::GenerateCaveSystem() {
    int k = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            mWalls[k].AddComponent<Component::Transform>();
            mWalls[k].AddComponent<Component::Mesh>();
            mWalls[k].GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
            if (map[i][j] == true) {
                mWalls[k].GetComponent<Component::Transform>()->Move((float)i, 0, (float)j);
            }
            else {
                mWalls[k].GetComponent<Component::Transform>()->mScale = glm::vec3(0.f, 0.f, 0.f);
            }
        }
    }
}