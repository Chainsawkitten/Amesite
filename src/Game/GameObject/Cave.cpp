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
#include <Engine/Geometry/Terrain.hpp>
#include "../Util/CaveGenerator.hpp"

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

Cave::Cave(Scene* scene, int width, int height, int seed, int percent, int iterations, int threshold, CaveGenerator::Coordinate playerPosition, std::vector<CaveGenerator::Coordinate> bossPositions) : SuperGameObject(scene) {
    mWidth = width;
    mHeight = height;
    mMap = nullptr;

    xScale = 5.f;
    zScale = 5.f;

    caveMap = new CaveGenerator::CaveMap(width, height, seed);

    caveMap->GenerateCaveMap(percent);

    caveMap->ProcessCaveMap(iterations);

    caveMap->RemoveSmallRooms(threshold);

    caveMap->CreateCircle(playerPosition, 7, false);

    caveMap->ConnectClosestRooms(true);

    for (auto& bossPosition : bossPositions) {
        caveMap->CreateCircle(bossPosition, 7, false);
    }

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
    map->AddComponent<Component::Material>();

    Geometry::Map* mapGeometry = Resources().CreateMap(mMap, glm::uvec2(mWidth, mHeight));

    mTypeMap = mapGeometry->GetTypeMap();

    map->GetComponent<Component::Mesh>()->geometry = mapGeometry;
    map->GetComponent<Component::Transform>()->Rotate(0.f, 0.f, 0.f);
    map->GetComponent<Component::Transform>()->Move(glm::vec3(xScale*static_cast<float>(mWidth) / 2.f, 0.f, zScale*static_cast<float>(mWidth) / 2.f));
    map->GetComponent<Component::Transform>()->scale = glm::vec3(xScale, 5.f, zScale);
    map->GetComponent<Component::Material>()->SetDiffuse("Resources/wall2_diff.png");
    map->GetComponent<Component::Material>()->SetSpecular("Resources/wall2_spec.png");

    heightMap = CreateEntity(scene);

    float** floatMap = new float*[width];
    for (int i = 0; i < width; i++) {
        floatMap[i] = new float[height];
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (mMap[i][j] == true)
                floatMap[i][j] = 1.0f;
            else
                floatMap[i][j] = 0.0f;
        }
    }

    heightMap = CreateEntity(scene);
    
    heightMap->AddComponent<Component::Mesh>();
    heightMap->AddComponent<Component::Transform>();
    heightMap->AddComponent<Component::Material>();
    heightMap->GetComponent<Component::Transform>()->Move(glm::vec3(xScale*(static_cast<float>(width)/2.f), -11.f, zScale*(static_cast<float>(height) / 2.f)));
    heightMap->GetComponent<Component::Transform>()->scale = glm::vec3((static_cast<float>(width)/2.f)*10, 7.f, (static_cast<float>(height) / 2.f) * 10);

    heightMap->GetComponent<Component::Mesh>()->geometry = new Geometry::Terrain(floatMap, width, height, glm::vec2(xScale, zScale));

    heightMap->GetComponent<Component::Material>()->SetDiffuse("Resources/wall2_diff.png");

}

Cave::~Cave() {
    for (int i = 0; i < 60; i++) {
        delete[] mMap[i];
    }
    delete caveMap;
    delete[] mMap;
}

bool ** Cave::GetCaveData()
{
    return mMap;
}
