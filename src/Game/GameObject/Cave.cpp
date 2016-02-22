#include "Cave.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/Cube.hpp>
#include <Geometry/Map.hpp>

#include "../Component/Controller.hpp"
#include "../Component/Health.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/SpotLight.hpp>
#include "../Util/CaveGenerator.hpp"

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;
bool** Cave::mMap = nullptr;


Cave::Cave(Scene* scene, int width, int height, int seed, int percent, int iterations, int threshold) : SuperGameObject(scene) {

    mWidth = width;
    mHeight = height;

    caveMap = new CaveGenerator::CaveMap(width, height, seed);

    caveMap->GenerateCaveMap(percent);

    caveMap->ProcessCaveMap(iterations);

    caveMap->RemoveSmallRooms(threshold);

    caveMap->ConnectClosestRooms(true);

    mMap = new bool*[width];
    for (int i = 0; i < width; i++) {
        mMap[i] = new bool[height];
    }

    for (int i = 0; i < width; i++) {   
        for (int j = 0; j < height; j++) {
            mMap[i][j] = caveMap->GetMap()[i][j];
        }
    }

    map = CreateEntity(scene);
    map->AddComponent<Component::Mesh>();
    map->AddComponent<Component::Transform>();
    map->AddComponent<Component::Physics>();
    map->AddComponent<Component::Material>();

    map->GetComponent<Component::Mesh>()->geometry = Resources().CreateMap(mMap, 1.f, glm::uvec2(width, height));
    map->GetComponent<Component::Transform>()->Rotate(0.f, 0.f, 0.f);
    map->GetComponent<Component::Physics>()->angularDragFactor = 0;
    map->GetComponent<Component::Physics>()->gravityFactor = 0;
    map->GetComponent<Component::Physics>()->velocity = glm::vec3(0.f, 0.f, 0.f);
    map->GetComponent<Component::Transform>()->Move(glm::vec3(5.f*static_cast<float>(width)/2.f, 0.f, 5.f*static_cast<float>(width) / 2.f));
    map->GetComponent<Component::Transform>()->scale = glm::vec3(5, 5, 5);
    map->GetComponent<Component::Material>()->SetDiffuse("Resources/wall2_diff.png");
    map->GetComponent<Component::Material>()->SetNormal("Resources/wall2_norm.png");
    map->GetComponent<Component::Material>()->SetSpecular("Resources/wall2_spec.png");
}

Cave::~Cave() {

    for (int i = 0; i < 60; i++) {

        delete mMap[i];

    }

    delete caveMap;

    delete[] mMap;

    Resources().FreeCube();

}
