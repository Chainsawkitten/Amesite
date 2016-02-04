#include "CaveSystem.hpp"

#include <Scene/Scene.hpp>

#include <Geometry/Cube.hpp>
#include <Resources.hpp>
#include <Component/Mesh.hpp>
#include <Component/Transform.hpp>
#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <Component/Transform.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/PointLight.hpp>
#include <Component/Collider2DCircle.hpp>
#include <Component/Physics.hpp>
#include "../Component/Health.hpp"

using namespace Caves;

const float CaveSystem::mMap[25][25] = {
    { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f },
    { 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f },
    { 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f },
    { 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f },
    { 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f },
    { 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f },
    { 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f },
    { 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f },
    { 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f },
    { 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f },
    { 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f },
    { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f },
    { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f },
    { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f },
    { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f },
    { 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f },
    { 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f },
    { 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f },
    { 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f },
    { 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f },
    { 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f },
    { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f },
    { 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f },
    { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f },
    { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f }
};

Entity* CaveSystem::GenerateCaveSystem(Scene* scene) {
    Entity* map = scene->CreateEntity();
    map->AddComponent<Component::Transform>()->scale = glm::vec3(2,2,2);
    map->AddComponent<Component::Physics>();
    map->GetComponent<Component::Physics>()->angularDragFactor = 0;
    map->GetComponent<Component::Physics>()->gravityFactor = 0;
    map->GetComponent<Component::Physics>()->velocity = glm::vec3(0.f, 0.f, 0.f);
    map->GetComponent<Component::Physics>()->angularVelocity = glm::vec3(0.f, 0.01f, 0.f);

    Geometry::Geometry3D* cube = Resources().CreateCube();
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 25; j++) {
            if (mMap[i][j] > 0.f) {
                Entity* wall = scene->CreateEntity();
                wall->AddComponent<Component::RelativeTransform>()->parentEntity = map;
                wall->AddComponent<Component::Mesh>();
                wall->AddComponent<Component::Collider2DCircle>()->radius = 1.f;
                wall->AddComponent<Component::Health>();
                //wall->AddComponent<Component::Physics>()->angularaAcceleration.y = 1;
                wall->GetComponent<Component::Mesh>()->geometry = cube;
                wall->GetComponent<Component::Transform>()->position = glm::vec3(float(j), 0.f, -float(i)) + glm::vec3(-25.f / 2.f, 0.f, 25.f / 2.f);
                //wall->GetComponent<Component::Transform>()->Rotate(45, 45, 0);

                //Entity* light = scene->CreateEntity();
                //light->AddComponent<Component::RelativeTransform>()->parentEntity = wall;
                //light->GetComponent<Component::Transform>()->position.y = 1;
                //light->AddComponent<Component::PointLight>();
                wall->GetComponent<Component::Transform>()->pitch = 45;
            }
            if (mMap[i][j] == -1.f) {

                Entity* wall = scene->CreateEntity();
                wall->AddComponent<Component::RelativeTransform>()->parentEntity = map;
                wall->AddComponent<Component::Mesh>();
                wall->GetComponent<Component::Mesh>()->geometry = cube;
                wall->GetComponent<Component::Transform>()->position = glm::vec3(float(j), 0.f, -float(i)) + glm::vec3(-25.f / 2.f, 0.f, 25.f / 2.f);
                wall->GetComponent<Component::Transform>()->scale = glm::vec3(0.2f, 0.2f, 0.2f);

            }
        }
    }
    return map;
}
