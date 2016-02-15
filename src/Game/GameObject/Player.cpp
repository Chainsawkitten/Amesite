#include "Player.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/Cube.hpp>

#include "../Component/Controller.hpp"
#include "../Component/Health.hpp"
#include "../Component/Spawner.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/SpotLight.hpp>
#include <Engine/Component/PointLight.hpp>

#include <Engine/Component/Animation.hpp>

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

Player::Player(Scene* scene) : SuperGameObject(scene) {
    mNode = mScene->CreateEntity();
    mEntityMap["node"] = mNode;
    mEntityVector.push_back(mNode);
    mNode->AddComponent<Component::Transform>()->scale *= 2.f;
    mNode->AddComponent<Component::Controller>()->speed = 3000.f;
    mNode->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Move);
    mNode->AddComponent<Component::Physics>()->velocityDragFactor = 3.f;
    mNode->AddComponent<Component::Health>();
    mNode->AddComponent<Component::Animation>();
    Component::Animation::AnimationClip* idleNode = mNode->GetComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleNode->CreateKeyFrame(glm::vec3(0.1f, 0.f, 0.f), 2.f, 1.f, 0, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(0.f, 0.1f, 0.1f), -1.f, -2.f, 1.f, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(0.1f, 0.1f, -0.1f), -2.f, 1.f, 2.f, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(-0.1f, 0.1f, 0.1f), 2.f, 2.f, 1.f, 1.5f, false, true);
    mNode->GetComponent<Component::Animation>()->Start("idle");

    mBody = mScene->CreateEntity();
    mEntityMap["body"] = mBody;
    mEntityVector.push_back(mBody);
    mBody->AddComponent<Component::RelativeTransform>()->parentEntity = mNode;
    mBody->AddComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    mBody->AddComponent<Component::Material>();
    mBody->AddComponent<Component::Controller>();
    mBody->AddComponent<Component::Animation>();

    mHead = mScene->CreateEntity();
    mEntityMap["head"] = mHead;
    mEntityVector.push_back(mHead);
    mHead->AddComponent<Component::RelativeTransform>()->parentEntity = mBody;
    mHead->GetComponent<Component::Transform>()->Move(0.f, 0.f, 1.5f);
    mHead->GetComponent<Component::Transform>()->scale *= 2.f;
    mHead->AddComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    mHead->AddComponent<Component::Material>();
    mHead->AddComponent<Component::Animation>();
    mHead->AddComponent<Component::Spawner>();
    mHead->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::ButtonShoot);

    Entity* spotLight = mScene->CreateEntity();
    mEntityMap["spotLight"] = spotLight;
    mEntityVector.push_back(spotLight);
    spotLight->AddComponent<Component::RelativeTransform>()->parentEntity = mHead;
    spotLight->AddComponent<Component::Animation>();
    spotLight->AddComponent<Component::SpotLight>()->coneAngle = 45.f;
    spotLight->GetComponent<Component::SpotLight>()->attenuation = 0.01f;
    spotLight->GetComponent<Component::SpotLight>()->color.g = 0.f;
}

Player::~Player() {
    Resources().FreeCube();
    Resources().FreeCube();
}

glm::vec3 Player::GetPosition() {
    return mNode->GetComponent<Component::Transform>()->GetWorldPosition();
}

float Player::GetHealth() {
    return mNode->GetComponent<Component::Health>()->health;
}

void Player::Shoot() {

}
