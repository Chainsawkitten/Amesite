#include "Pillar.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>


#include <Engine/Component/Transform.hpp>
#include <Engine/Component/RelativeTransform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/PointLight.hpp>
#include <Engine/Component/Physics.hpp>
#include "../Component/Update.hpp"
#include "../Component/Spawner.hpp"
#include "../Component/LifeTime.hpp"

#include "../Util/GameEntityFactory.hpp"
#include "../Util/ControlSchemes.hpp"

#include "../GameObject/PillarBall.hpp"

using namespace GameObject;

Pillar::Pillar(Scene* scene, glm::vec3 bossPosition) : SuperGameObject(scene) {
    mBossPosition = bossPosition;

    node = CreateEntity();
    node->AddComponent<Component::Transform>();
    node->AddComponent<Component::Material>();
    node->GetComponent<Component::Material>()->SetDiffuse("Resources/pillar_diff.png");
    node->GetComponent<Component::Material>()->SetGlow("Resources/pillar_glow.png");
    node->AddComponent<Component::Mesh>()->geometry = mPillarModel = Resources().CreateOBJModel("Resources/pillar.obj");
    node->AddComponent<Component::Update>()->updateFunction = std::bind(&Pillar::mUpdateFunction, this);
    node->AddComponent<Component::Spawner>()->delay = 1.5f;

    mState = State::DEACTIVATED;
    mLastState = State::DEACTIVATED;

    mLightModel = Resources().CreateOBJModel("Resources/portal.obj");

}

Pillar::~Pillar() {
    Resources().FreeOBJModel(mPillarModel);
    Resources().FreeOBJModel(mLightModel);
}

void Pillar::SetState(State state) {
    mState = state;
}

Pillar::State Pillar::GetState() {
    return mState;
}

void Pillar::mUpdateFunction() {

    // Check state
    if (mLastState != mState) {
        if (mLastState == State::ACTIVE) {
            mLight->Kill();
        } else {
            CreateLight();
        }
        mLastState = mState;
    }

    // Update
    if (mState == State::ACTIVE) {
        Component::Spawner* spawnerComponent = node->GetComponent<Component::Spawner>();
        if (spawnerComponent->timeSinceSpawn >= spawnerComponent->delay) {
            GameEntityCreator().CreatePillarBall(node->GetComponent<Component::Transform>()->GetWorldPosition() - glm::vec3(0.f, 10.f, 0.f), glm::vec3(0.f, 10.f, 0.f));
            spawnerComponent->timeSinceSpawn = 0.0f;
        }
    } else {
        Component::Spawner* spawnerComponent = node->GetComponent<Component::Spawner>();
        if (spawnerComponent->timeSinceSpawn >= spawnerComponent->delay * 2.f) {
            glm::vec3 velocity = mBossPosition - node->GetComponent<Component::Transform>()->position;
            GameObject::PillarBall* ball = GameEntityCreator().CreatePillarBall(node->GetComponent<Component::Transform>()->GetWorldPosition() + glm::vec3(0.f, 40.f, 0.f), velocity/10.f);
            ball->node->GetComponent<Component::Transform>()->scale *= 0.2;
            spawnerComponent->timeSinceSpawn = 0.0f;
        }
    }
}

void Pillar::CreateLight() {
    mLight = CreateEntity();
    mLight->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    mLight->GetComponent<Component::RelativeTransform>()->Move(0.f, 8.f, 0.f);
    mLight->GetComponent<Component::RelativeTransform>()->scale *= 0.2f;
    mLight->AddComponent<Component::Material>();
    mLight->AddComponent<Component::Mesh>()->geometry = mLightModel;
    mLight->AddComponent<Component::PointLight>()->attenuation = 1.f;
}
