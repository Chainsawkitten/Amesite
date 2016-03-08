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

#include "Geometry/Plane.hpp"

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
    map->GetComponent<Component::Transform>()->Move(glm::vec3(scaleFactor*static_cast<float>(mWidth) / 2.f - scaleFactor / 2.f, 3.f, scaleFactor*static_cast<float>(mWidth) / 2.f - scaleFactor / 2.f));
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

    for (int i = 0; i < mHeight; i++)
        delete[] floatMap[i];
    delete[] floatMap;

    //Create the border
    mBorder = new Geometry::Plane();

    mTopBorder = CreateEntity();
    mTopBorder->AddComponent<Component::Mesh>();
    mTopBorder->AddComponent<Component::Material>();
    mTopBorder->AddComponent<Component::Transform>()->Move(glm::vec3(scaleFactor*(static_cast<float>(width) / 2.f), 3, scaleFactor*(static_cast<float>(height) / 2.f)));
    mTopBorder->GetComponent<Component::Transform>()->Move(glm::vec3(0, 0, scaleFactor*(static_cast<float>(width) / 2.f) + 45.f));
    mTopBorder->GetComponent<Component::Transform>()->scale = glm::vec3(1000, 100, 1);
    mTopBorder->GetComponent<Component::Transform>()->Rotate(0, -90, 0);
    mTopBorder->GetComponent<Component::Mesh>()->geometry = mBorder;

    mBottomBorder = CreateEntity();
    mBottomBorder->AddComponent<Component::Mesh>();
    mBottomBorder->AddComponent<Component::Material>();
    mBottomBorder->AddComponent<Component::Transform>()->Move(glm::vec3(scaleFactor*(static_cast<float>(width) / 2.f), 3, scaleFactor*(static_cast<float>(height) / 2.f)));
    mBottomBorder->GetComponent<Component::Transform>()->Move(glm::vec3(0, 0, -scaleFactor*(static_cast<float>(width) / 2.f) - 50.f));
    mBottomBorder->GetComponent<Component::Transform>()->scale = glm::vec3(1000, 100, 1);
    mBottomBorder->GetComponent<Component::Transform>()->Rotate(0, -90, 0);
    mBottomBorder->GetComponent<Component::Mesh>()->geometry = mBorder;

    mRightBorder = CreateEntity();
    mRightBorder->AddComponent<Component::Mesh>();
    mRightBorder->AddComponent<Component::Material>();
    mRightBorder->AddComponent<Component::Transform>()->Move(glm::vec3(scaleFactor*(static_cast<float>(width) / 2.f), 3, scaleFactor*(static_cast<float>(height) / 2.f)));
    mRightBorder->GetComponent<Component::Transform>()->Move(glm::vec3(scaleFactor*(static_cast<float>(width) / 2.f) + 45.f, 0, 0));
    mRightBorder->GetComponent<Component::Transform>()->scale = glm::vec3(100, 1000, 1);
    mRightBorder->GetComponent<Component::Transform>()->Rotate(0, -90, 0);
    mRightBorder->GetComponent<Component::Mesh>()->geometry = mBorder;

    mLeftBorder = CreateEntity();
    mLeftBorder->AddComponent<Component::Mesh>();
    mLeftBorder->AddComponent<Component::Material>();
    mLeftBorder->AddComponent<Component::Transform>()->Move(glm::vec3(scaleFactor*(static_cast<float>(width) / 2.f), 3, scaleFactor*(static_cast<float>(height) / 2.f)));
    mLeftBorder->GetComponent<Component::Transform>()->Move(glm::vec3(scaleFactor*(-static_cast<float>(width) / 2.f) - 50.f, 0, 0));
    mLeftBorder->GetComponent<Component::Transform>()->scale = glm::vec3(100, 1000, 1);
    mLeftBorder->GetComponent<Component::Transform>()->Rotate(0, -90, 0);
    mLeftBorder->GetComponent<Component::Mesh>()->geometry = mBorder;

    //Set the texture
    std::string texture = "Resources/wall_gray.png";
    mTopBorder->GetComponent<Component::Material>()->SetDiffuse(texture.c_str());
    mTopBorder->GetComponent<Component::Material>()->SetSpecular(texture.c_str());
    mBottomBorder->GetComponent<Component::Material>()->SetDiffuse(texture.c_str());
    mBottomBorder->GetComponent<Component::Material>()->SetSpecular(texture.c_str());
    mRightBorder->GetComponent<Component::Material>()->SetDiffuse(texture.c_str());
    mRightBorder->GetComponent<Component::Material>()->SetSpecular(texture.c_str());
    mLeftBorder->GetComponent<Component::Material>()->SetDiffuse(texture.c_str());
    mLeftBorder->GetComponent<Component::Material>()->SetSpecular(texture.c_str());

}

Cave::~Cave() {
    
    for (int i = 0; i < mHeight - 1; i++)
        delete[] mMap[i];
    delete[] mMap;

    delete caveMap;
    delete mBorder;

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

glm::vec3 Cave::PointCollide(glm::vec3 point, glm::vec3 velocity, float deltaTime) {
    unsigned int x = glm::floor((point + velocity * deltaTime).x / scaleFactor);
    unsigned int z = glm::floor((point + velocity * deltaTime).z / scaleFactor);
    
    return CellCollide(((point + velocity * deltaTime).x) / scaleFactor - x, ((point + velocity * deltaTime).z) / scaleFactor - z, x, z);
}

glm::vec3 Cave::CellCollide(float xPos, float yPos, int x, int y) {

    if (x >= 0 && y >= 0 && x < 89 && y < 89)
        switch (this->mTypeMap[x][y]) {

        case 1:
            if (yPos <= 0.5f - xPos)
                return glm::vec3(1, 0, 1);
            break;
        case 2:
            if (xPos >= 0.5f + yPos)
                return glm::vec3(-1, 0, 1);
            break;
        case 3:
            if (yPos <= 0.5f)
                return glm::vec3(0, 0, 1);
            break;
        case 4:
            if (!(yPos <= 1.5f - xPos))
                return glm::vec3(-1, 0, -1);
            break;
        case 6:
            if (xPos >= 0.5f)
                return glm::vec3(-1, 0, 0);
            break;
        case 7:
            if (yPos <= xPos + 0.5f)
                return glm::vec3(-1, 0, 1);
            break;
        case 8:
            if (!(yPos <= xPos + 0.5f))
                return glm::vec3(1, 0, -1);
            break;
        case 9:
            if (xPos <= 0.5f)
                return glm::vec3(1, 0, 0);
            break;
        case 11:
            if (yPos <= 1.5f - xPos)
                return glm::vec3(1, 0, 1);
            break;
        case 12:
            if (yPos >= 0.5f)
                return glm::vec3(0, 0, -1);
            break;
        case 13:
            if (!(xPos >= 0.5f + yPos))
                return glm::vec3(1, 0, -1);
            break;
        case 14:
            if (!(yPos <= 0.5f - xPos))
                return glm::vec3(-1, 0, -1);
            break;
        case 15:
            if (yPos <= xPos + 0.5f)
                return glm::vec3(-1, -1, -1);
            break;

        }

    return glm::vec3(0, 0, 0);

}

bool Cave::GridCollide(Entity* entity, float deltaTime) {

    Component::Transform* transform = entity->GetComponent<Component::Transform>();
    Component::Physics* physics = entity->GetComponent<Component::Physics>();

    glm::vec3 velocity = physics->velocity;
    velocity += physics->acceleration * deltaTime;
    velocity -= physics->velocity * physics->velocityDragFactor * deltaTime;

    glm::vec3 width = glm::vec3(transform->entity->GetComponent<Component::Collider2DCircle>()->radius * transform->GetWorldScale().x * 1.f, 0, 0);
    glm::vec3 height = glm::vec3(0, 0, transform->entity->GetComponent<Component::Collider2DCircle>()->radius * transform->GetWorldScale().x * 1.f);

    glm::vec3 n;
    float bounce = 2.f;

    n = PointCollide(transform->CalculateWorldPosition() + height + width, velocity, deltaTime);

    if (n != glm::vec3(0, 0, 0)) {

        n = glm::normalize(n);
        glm::vec3 newVelocity = velocity - 2.f * (glm::dot(velocity, n) * n);

        bool c0 = PointCollide(transform->CalculateWorldPosition() + height + width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);
        bool c1 = PointCollide(transform->CalculateWorldPosition() - height - width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);
        bool c2 = PointCollide(transform->CalculateWorldPosition() - height + width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);
        bool c3 = PointCollide(transform->CalculateWorldPosition() + height - width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);

        if (c0 && c1 && c2 && c3) {

            physics->velocity = velocity - 2.f * (glm::dot(velocity, n) * n);
            physics->acceleration = physics->acceleration - 2.f * (glm::dot(physics->acceleration, n) * n);

            physics->velocity /= bounce;
            physics->acceleration /= bounce;

            return true;

        }

        physics->velocity = glm::vec3(0, 0, 0);
        physics->acceleration = glm::vec3(0, 0, 0);

    }

    n = PointCollide(transform->CalculateWorldPosition() + height - width, velocity, deltaTime);

    if (n != glm::vec3(0, 0, 0)) {

        n = glm::normalize(n);
        glm::vec3 newVelocity = velocity - 2.f * (glm::dot(velocity, n) * n);

        bool c0 = PointCollide(transform->CalculateWorldPosition() + height + width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);
        bool c1 = PointCollide(transform->CalculateWorldPosition() - height - width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);
        bool c2 = PointCollide(transform->CalculateWorldPosition() - height + width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);
        bool c3 = PointCollide(transform->CalculateWorldPosition() + height - width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);

        if (c0 && c1 && c2 && c3) {

            physics->velocity = velocity - 2.f * (glm::dot(velocity, n) * n);
            physics->acceleration = physics->acceleration - 2.f * (glm::dot(physics->acceleration, n) * n);

            physics->velocity /= bounce;
            physics->acceleration /= bounce;

            return true;

        }

        physics->velocity = glm::vec3(0, 0, 0);
        physics->acceleration = glm::vec3(0, 0, 0);

    }

    n = PointCollide(transform->CalculateWorldPosition() - height - width, velocity, deltaTime);

    if (n != glm::vec3(0, 0, 0)) {

        n = glm::normalize(n);
        glm::vec3 newVelocity = velocity - 2.f * (glm::dot(velocity, n) * n);

        bool c0 = PointCollide(transform->CalculateWorldPosition() + height + width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);
        bool c1 = PointCollide(transform->CalculateWorldPosition() - height - width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);
        bool c2 = PointCollide(transform->CalculateWorldPosition() - height + width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);
        bool c3 = PointCollide(transform->CalculateWorldPosition() + height - width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);

        if (c0 && c1 && c2 && c3) {

            physics->velocity = velocity - 2.f * (glm::dot(velocity, n) * n);
            physics->acceleration = physics->acceleration - 2.f * (glm::dot(physics->acceleration, n) * n);

            physics->velocity /= bounce;
            physics->acceleration /= bounce;

            return true;

        }

        physics->velocity = glm::vec3(0, 0, 0);
        physics->acceleration = glm::vec3(0, 0, 0);

    }

    n = PointCollide(transform->CalculateWorldPosition() - height + width, velocity, deltaTime);

    if (n != glm::vec3(0, 0, 0)) {

        n = glm::normalize(n);
        glm::vec3 newVelocity = velocity - 2.f * (glm::dot(velocity, n) * n);

        bool c0 = PointCollide(transform->CalculateWorldPosition() + height + width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);
        bool c1 = PointCollide(transform->CalculateWorldPosition() - height - width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);
        bool c2 = PointCollide(transform->CalculateWorldPosition() - height + width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);
        bool c3 = PointCollide(transform->CalculateWorldPosition() + height - width, newVelocity, deltaTime) == glm::vec3(0, 0, 0);

        if (c0 && c1 && c2 && c3) {

            physics->velocity = velocity - 2.f * (glm::dot(velocity, n) * n);
            physics->acceleration = physics->acceleration - 2.f * (glm::dot(physics->acceleration, n) * n);

            physics->velocity /= bounce;
            physics->acceleration /= bounce;

            return true;

        }

        physics->velocity = glm::vec3(0, 0, 0);
        physics->acceleration = glm::vec3(0, 0, 0);

    }

    return false;

}
