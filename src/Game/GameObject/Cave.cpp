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

    Geometry::Map* mapGeometry = Resources().CreateMap(mMap, glm::uvec2(mWidth, mHeight));

    mTypeMap = mapGeometry->GetTypeMap();

    map->GetComponent<Component::Mesh>()->geometry = mapGeometry;
    map->GetComponent<Component::Transform>()->Rotate(0.f, 0.f, 0.f);
    map->GetComponent<Component::Transform>()->Move(glm::vec3(scaleFactor*static_cast<float>(mWidth) / 2.f - scaleFactor / 2.f, 0.f, scaleFactor*static_cast<float>(mWidth) / 2.f - scaleFactor / 2.f));
    map->GetComponent<Component::Transform>()->scale = glm::vec3(scaleFactor, 5.f, scaleFactor);
    map->GetComponent<Component::Material>()->SetDiffuse("Resources/wall2_spec.png");
    map->GetComponent<Component::Material>()->SetSpecular("Resources/wall2_spec.png");

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
    heightMap->GetComponent<Component::Material>()->SetDiffuse("Resources/wall2_diff.png");
    heightMap->GetComponent<Component::Material>()->SetSpecular("Resources/wall2_spec.png");

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

bool Cave::PointCollide(glm::vec3 point, glm::vec3 velocity, float deltaTime) {
    unsigned int x = glm::floor((point + velocity * deltaTime).x / scaleFactor);
    unsigned int z = glm::floor((point + velocity * deltaTime).z / scaleFactor);

    return CellCollide(((point + velocity * deltaTime).x) / scaleFactor - x, ((point + velocity * deltaTime).z) / scaleFactor - z, x, z);
}

bool Cave::CellCollide(float xPos, float yPos, int x, int y) {

    if (x >= 0 && y >= 0)
        switch (this->mTypeMap[x][y]) {

        case 1:
            if (yPos <= 0.5f - xPos)
                return true;
            break;
        case 2:
            if (xPos >= 0.5f + yPos)
                return true;
            break;
        case 3:
            if (yPos <= 0.5f)
                return true;
            break;
        case 4:
            if (!(yPos <= 1.5f - xPos))
                return true;
            break;
        case 6:
            if (xPos >= 0.5f)
                return true;
            break;
        case 7:
            if (yPos <= xPos + 0.5f)
                return true;
            break;
        case 8:
            if (!(yPos <= xPos + 0.5f))
                return true;
            break;
        case 9:
            if (xPos <= 0.5f)
                return true;
            break;
        case 11:
            if (yPos <= 1.5f - xPos)
                return true;
            break;
        case 12:
            if (yPos >= 0.5f)
                return true;
            break;
        case 13:
            if (!(xPos >= 0.5f + yPos))
                return true;
            break;
        case 14:
            if (!(yPos <= 0.5f - xPos))
                return true;
            break;
        case 15:
            if (yPos <= xPos + 0.5f)
                return true;
            break;

        }

    return false;

}

bool Cave::GridCollide(Entity* entity, float deltaTime) {

    Component::Transform* transform = entity->GetComponent<Component::Transform>();
    Component::Physics* physics = entity->GetComponent<Component::Physics>();

    glm::vec3 velocity = physics->velocity;
    velocity += physics->acceleration * deltaTime;
    velocity -= physics->velocity * physics->velocityDragFactor * deltaTime;

    glm::vec3 width = glm::vec3(transform->entity->GetComponent<Component::Collider2DCircle>()->radius * transform->GetWorldScale().x * 1.f, 0, 0);
    glm::vec3 height = glm::vec3(0, 0, transform->entity->GetComponent<Component::Collider2DCircle>()->radius * transform->GetWorldScale().x * 1.f);

    bool c0 = PointCollide(transform->CalculateWorldPosition() - width - height, velocity, deltaTime);
    bool c1 = PointCollide(transform->CalculateWorldPosition() + width - height, velocity, deltaTime);
    bool c2 = PointCollide(transform->CalculateWorldPosition() + width + height, velocity, deltaTime);
    bool c3 = PointCollide(transform->CalculateWorldPosition() - width + height, velocity, deltaTime);

    if (c0 || c1 || c2 || c3) {

        physics->velocity *= glm::vec3(0, 0, 0);
        physics->acceleration *= glm::vec3(0, 0, 0);
        return true;

    }

    return false;

}
