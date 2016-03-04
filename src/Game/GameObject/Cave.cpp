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

    mBossRadius = 7;

    scaleFactor = 5.f;

    caveMap = new CaveGenerator::CaveMap(height, width, seed);

    caveMap->GenerateCaveMap(percent);

    caveMap->ProcessCaveMap(iterations);

    caveMap->DetectRooms();

    caveMap->RemoveSmallRooms(threshold);

    caveMap->CreateCircle(playerPosition, mBossRadius, false);

    for (auto& bossPosition : bossPositions) {
        caveMap->CreateCircle(bossPosition, mBossRadius, false);
    }

    caveMap->DetectRooms();

    caveMap->ConnectClosestRooms(true);

    mMap = new bool*[height];
    for (int i = 0; i < height; i++) {
        mMap[i] = new bool[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            mMap[i][j] = caveMap->GetMap()[i][j];
        }
    }

    map = CreateEntity();
    map->AddComponent<Component::Mesh>();
    map->AddComponent<Component::Transform>();
    map->AddComponent<Component::Material>();

    map->GetComponent<Component::Mesh>()->geometry = Resources().CreateMap(mMap, glm::uvec2(mWidth, mHeight), 15.f);
    map->GetComponent<Component::Transform>()->Rotate(0.f, 0.f, 0.f);
    map->GetComponent<Component::Transform>()->Move(glm::vec3(scaleFactor*static_cast<float>(mWidth) / 2.f, 9.f, scaleFactor*static_cast<float>(mWidth) / 2.f));
    map->GetComponent<Component::Transform>()->scale = glm::vec3(scaleFactor, 5.f, scaleFactor);
    map->GetComponent<Component::Material>()->SetDiffuse("Resources/wall_gray.png");
    map->GetComponent<Component::Material>()->SetSpecular("Resources/wall_gray.png");

    heightMap = CreateEntity();

    float** floatMap = new float*[height];
    for (int i = 0; i < height; i++) {
        floatMap[i] = new float[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (mMap[i][j] == true)
                floatMap[j][i] = 1.0f;
            else
                floatMap[j][i] = 0.0f;
        }
    }

    heightMap = CreateEntity();
    
    heightMap->AddComponent<Component::Mesh>();
    heightMap->AddComponent<Component::Transform>();
    heightMap->AddComponent<Component::Material>();
    heightMap->GetComponent<Component::Transform>()->Move(glm::vec3(scaleFactor*(static_cast<float>(width)/2.f)+1.f, -11.f, scaleFactor*(static_cast<float>(height) / 2.f) + 1.f));
    heightMap->GetComponent<Component::Transform>()->scale = glm::vec3((static_cast<float>(width)/2.f)*10, 7.f, (static_cast<float>(height) / 2.f) * 10);

    heightMap->GetComponent<Component::Mesh>()->geometry = new Geometry::Terrain(floatMap, height, width, glm::vec2(scaleFactor, scaleFactor));
    heightMap->GetComponent<Component::Material>()->SetDiffuse("Resources/ground_sand.png");
    heightMap->GetComponent<Component::Material>()->SetSpecular("Resources/ground_sand.png");

}

Cave::~Cave() {
    delete caveMap;
}

int Cave::GetWidth() const {
    return mWidth;
}

int Cave::GetHeight() const {
    return mHeight;
}
int Cave::GetBossRoomRadius() const {
    return mBossRadius;
}

bool ** Cave::GetCaveData() const {
    return mMap;
}

int Cave::PointCollide(glm::vec3 point, glm::vec3 velocity, float deltaTime) {
    int oldX = static_cast<int>(point.x / scaleFactor);
    int oldZ = static_cast<int>(point.z / scaleFactor);
    int newX = static_cast<int>((point + velocity * deltaTime).x / scaleFactor);
    int newZ = static_cast<int>((point + velocity * deltaTime).z / scaleFactor);

    float X = (newX - oldX) / velocity.x;
    float Z = (newZ - oldZ) / velocity.z;

    if (newX >= mWidth || newX < 0 || newZ >= mHeight || newZ < 0)
        return -2;

    //We check if we moved to another cell in the grid.
    if (mMap[abs(newZ)][abs(newX)]) {
        //We collide in X
        if (X > Z) {

            if (oldX != newX) {
                return 0;
            }
            else if (oldZ != newZ) {
                return 1;
            }
        }
        //We collide in Z
        else {
            if (oldZ != newZ) {
                return 1;
            }
            else if (oldX != newX) {
                return 0;
            }
        }
    }
    return -1;
}

bool Cave::GridCollide(Entity* entity, float deltaTime) {

    Component::Transform* transform = entity->GetComponent<Component::Transform>();
    Component::Physics* physics = entity->GetComponent<Component::Physics>();

    glm::vec3 velocity = physics->velocity;
    velocity += physics->acceleration * deltaTime;
    velocity -= physics->velocity * physics->velocityDragFactor * deltaTime;

    glm::vec3 width = glm::vec3(transform->entity->GetComponent<Component::Collider2DCircle>()->radius * transform->GetWorldScale().x * 1.f, 0, 0);
    glm::vec3 height = glm::vec3(0, 0, transform->entity->GetComponent<Component::Collider2DCircle>()->radius * transform->GetWorldScale().x * 1.f);

    //glm::vec3 width = glm::vec3(2.9f, 0.f, 0.f);
    //glm::vec3 height = glm::vec3(0.f, 0.f, 2.9f);

    int c0 = PointCollide(transform->CalculateWorldPosition() - width - height, velocity, deltaTime);
    int c1 = PointCollide(transform->CalculateWorldPosition() + width - height, velocity, deltaTime);
    int c2 = PointCollide(transform->CalculateWorldPosition() + width + height, velocity, deltaTime);
    int c3 = PointCollide(transform->CalculateWorldPosition() - width + height, velocity, deltaTime);

    switch (c0) {

    case 0:
        physics->velocity *= glm::vec3(0, 0, 1);
        physics->acceleration *= glm::vec3(0, 0, 1);
        break;

    case 1:
        physics->velocity *= glm::vec3(1, 0, 0);
        physics->acceleration *= glm::vec3(1, 0, 0);
        break;

    }
    switch (c1) {

    case 0:
        physics->velocity *= glm::vec3(0, 0, 1);
        physics->acceleration *= glm::vec3(0, 0, 1);
        break;

    case 1:
        physics->velocity *= glm::vec3(1, 0, 0);
        physics->acceleration *= glm::vec3(1, 0, 0);
        break;

    }
    switch (c2) {

    case 0:
        physics->velocity *= glm::vec3(0, 0, 1);
        physics->acceleration *= glm::vec3(0, 0, 1);
        break;

    case 1:
        physics->velocity *= glm::vec3(1, 0, 0);
        physics->acceleration *= glm::vec3(1, 0, 0);
        break;

    }
    switch (c3) {

    case 0:
        physics->velocity *= glm::vec3(0, 0, 1);
        physics->acceleration *= glm::vec3(0, 0, 1);
        break;

    case 1:
        physics->velocity *= glm::vec3(1, 0, 0);
        physics->acceleration *= glm::vec3(1, 0, 0);
        break;

    }

    //if (c0 == -2 || c1 == -2 || c2 == -2 || c3 == -2)
    //    if (entity->GetComponent<Component::LifeTime>() != nullptr)
    //        entity->GetComponent<Component::LifeTime>()->lifeTime = 0.f;

    if (c0 != -1 || c1 != -1 || c2 != -1 || c3 != -1)
        return true;


    return false;

}
