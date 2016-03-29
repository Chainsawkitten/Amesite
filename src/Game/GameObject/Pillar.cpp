#include "Pillar.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include <Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/OBJModel.hpp>


#include <Component/Transform.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/Mesh.hpp>
#include <Component/Material.hpp>
#include <Component/PointLight.hpp>
#include <Component/Physics.hpp>
#include <Component/Animation.hpp>
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
    node->GetComponent<Component::Transform>()->yaw = (rand() % 11) * 36.f;
    node->AddComponent<Component::Material>();
    node->GetComponent<Component::Material>()->SetDiffuse("Resources/Pillar_Albedo.png");
    node->GetComponent<Component::Material>()->SetNormal("Resources/Pillar_NM.png");
    node->GetComponent<Component::Material>()->SetGlow("Resources/Pillar_Glow2.png");
    node->AddComponent<Component::Mesh>()->geometry = mPillarModel = Resources().CreateOBJModel("Resources/Pillar2.obj");
    //node->GetComponent<Component::Material>()->SetDiffuse("Resources/pillar_diff.png");
    //node->GetComponent<Component::Material>()->SetGlow("Resources/pillar_glow.png");
    //node->AddComponent<Component::Mesh>()->geometry = mPillarModel = Resources().CreateOBJModel("Resources/pillar.obj");
    node->AddComponent<Component::Update>()->updateFunction = std::bind(&Pillar::mUpdateFunction, this);
    node->AddComponent<Component::Spawner>()->delay = 1.5f;

    mState = State::DEACTIVATED;
    mLastState = State::DEACTIVATED;

    mLightModel = Resources().CreateOBJModel("Resources/pylon.obj");

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
            float maxHeight = 200.f;
            GameObject::PillarBall* ball = GameEntityCreator().CreatePillarBall(node->GetComponent<Component::Transform>()->GetWorldPosition() + glm::vec3(0.f, 7.f, 0.f), node->GetComponent<Component::Transform>()->GetWorldPosition() + glm::vec3(0.f, maxHeight, 0.f));
            ball->maxHeight = maxHeight;
            spawnerComponent->timeSinceSpawn = 0.0f;
        }
    } else {
        Component::Spawner* spawnerComponent = node->GetComponent<Component::Spawner>();
        if (spawnerComponent->timeSinceSpawn >= spawnerComponent->delay * 2.f) {
            GameObject::PillarBall* ball = GameEntityCreator().CreatePillarBall(node->GetComponent<Component::Transform>()->GetWorldPosition() + glm::vec3(0.f, 7.f, 0.f), mBossPosition);
            ball->maxHeight = 20.f;
            ball->node->GetComponent<Component::Transform>()->scale *= 0.2;
            spawnerComponent->timeSinceSpawn = 0.0f;
        }
    }
}

void Pillar::CreateLight() {
    mLight = CreateEntity();
    mLight->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    mLight->GetComponent<Component::RelativeTransform>()->Move(0.f, 8.f, 0.f);
    mLight->GetComponent<Component::RelativeTransform>()->scale *= 0.17f;
    mLight->AddComponent<Component::Material>();
    mLight->GetComponent<Component::Material>()->SetDiffuse("Resources/DefaultBlue.png");
    mLight->GetComponent<Component::Material>()->SetSpecular("Resources/enemy_spec.png");
    mLight->GetComponent<Component::Material>()->SetGlow("Resources/DefaultSpecular.png");
    mLight->AddComponent<Component::Mesh>()->geometry = mLightModel;
    mLight->AddComponent<Component::PointLight>()->attenuation = 1.f;
    mLight->GetComponent<Component::PointLight>()->color = glm::vec3(109.f, 242.f, 207.f) * 0.01f;
    Component::Animation::AnimationClip* idleNode = mLight->AddComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleNode->CreateKeyFrame(glm::vec3(0.f, 0.f, 0.f), 0.f, 0.f, 0, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(0.f, 0.5f, 0.f), 0.f, 0.f, 0.f, 1.5f, false, true);
    mLight->GetComponent<Component::Animation>()->Start("idle");
}
