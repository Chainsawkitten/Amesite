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
    Entity* node = mScene->CreateEntity();
    mEntityMap["node"] = node;
    mEntityVector.push_back(node);
    node->AddComponent<Component::Transform>()->scale = glm::vec3(2,2,2);
    node->AddComponent<Component::Controller>()->speed = 3000.f;
    node->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Move);
    node->AddComponent<Component::Physics>()->velocityDragFactor = 3.f;

    Entity* body = mScene->CreateEntity();
    mEntityMap["body"] = body;
    mEntityVector.push_back(body);
    body->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    body->AddComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    body->AddComponent<Component::Material>();
    body->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::ArrowKeyRotate);
    body->AddComponent<Component::Animation>();
    Component::Animation::AnimationClip* idleBody = body->GetComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleBody->CreateKeyFrame(glm::vec3(0.1, 0, 0), 2, 1, 0, 1.5, false, true);
    idleBody->CreateKeyFrame(glm::vec3(0, 0.1, 0.1), -1, -2, 1, 1.5, false, true);
    idleBody->CreateKeyFrame(glm::vec3(0.1, 0.1, -0.1), -2, 1, 2, 1.5, false, true);
    idleBody->CreateKeyFrame(glm::vec3(-0.1, 0.1, 0.1), 2, 2, 1, 1.5, false, true);
    body->GetComponent<Component::Animation>()->Start("idle");

    Entity* head = mScene->CreateEntity();
    mEntityMap["head"] = head;
    mEntityVector.push_back(head);
    head->AddComponent<Component::RelativeTransform>()->parentEntity = body;
    head->GetComponent<Component::Transform>()->Move(0.f, 0.2, 0.5);
    head->GetComponent<Component::Transform>()->Rotate(45.f, 0.f, 0.f);
    //head->GetComponent<Component::Transform>()->scale = glm::vec3(0.5f, 0.5f, 0.5f);
    head->AddComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    head->AddComponent<Component::Material>();
    head->AddComponent<Component::Animation>();
    Component::Animation::AnimationClip* idleHead = head->GetComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleHead->CreateKeyFrame(glm::vec3(0.1, 0, 0), 2, 1, 0, 1.5, false, true);
    idleHead->CreateKeyFrame(glm::vec3(0, 0.1, 0.1), -1, -2, 1, 1.5, false, true);
    idleHead->CreateKeyFrame(glm::vec3(0.1, 0.1, -0.1), -2, 1, 2, 1.5, false, true);
    idleHead->CreateKeyFrame(glm::vec3(-0.1, 0.1, 0.1), 2, 2, 1, 1.5, false, true);
    head->GetComponent<Component::Animation>()->Start("idle");

    Entity* tail = mScene->CreateEntity();
    mEntityMap["tail"] = tail;
    mEntityVector.push_back(tail);
    tail->AddComponent<Component::RelativeTransform>()->parentEntity = body;
    tail->GetComponent<Component::Transform>()->Move(0.f, 1.0, -0.5f);
    //tail->GetComponent<Component::Transform>()->scale = glm::vec3(0.3f, 0.3f, 0.3f);
    tail->AddComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    tail->AddComponent<Component::Material>();
    tail->AddComponent<Component::Animation>();
    Component::Animation::AnimationClip* idleTail = tail->GetComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleTail->CreateKeyFrame(glm::vec3(0.1, 0, 0), 2, 1, 0, 1.5, false, true);
    idleTail->CreateKeyFrame(glm::vec3(0, 0.1, 0.1), -1, -2, 1, 1.5, false, true);
    idleTail->CreateKeyFrame(glm::vec3(0.1, 0.1, -0.1), -2, 1, 2, 1.5, false, true);
    idleTail->CreateKeyFrame(glm::vec3(-0.1, 0.1, 0.1), 2, 2, 1, 1.5, false, true);
    tail->GetComponent<Component::Animation>()->Start("idle");

    Entity* leftLight = mScene->CreateEntity();
    mEntityMap["leftLight"] = leftLight;
    mEntityVector.push_back(leftLight);
    leftLight->AddComponent<Component::RelativeTransform>()->parentEntity = body;
    leftLight->GetComponent<Component::Transform>()->Move(0.5f, 1.f, -0.3f);
    leftLight->AddComponent<Component::SpotLight>()->coneAngle = 45.f;
    leftLight->GetComponent<Component::SpotLight>()->attenuation = 0.01f;
    leftLight->GetComponent<Component::SpotLight>()->color.g = 0.f;

    Entity* rightLight = mScene->CreateEntity();
    mEntityMap["rightLight"] = rightLight;
    mEntityVector.push_back(rightLight);
    rightLight->AddComponent<Component::RelativeTransform>()->parentEntity = body;
    rightLight->GetComponent<Component::Transform>()->Move(-0.5f, 1.f, -0.3f);
    rightLight->AddComponent<Component::SpotLight>()->coneAngle = 45.f;
    rightLight->GetComponent<Component::SpotLight>()->attenuation = 0.01f;
    rightLight->GetComponent<Component::SpotLight>()->color.g = 0.f;

    Entity* topLight = mScene->CreateEntity();
    mEntityMap["topLight"] = topLight;
    mEntityVector.push_back(topLight);
    topLight->AddComponent<Component::RelativeTransform>()->parentEntity = tail;
    topLight->GetComponent<Component::Transform>()->Move(0.f, 2.f, 0.f);
    topLight->AddComponent<Component::PointLight>()->color.r = 0.f;

    //body->AddComponent<Component::Collider2DCircle>()->radius = 0.5;
    //body->AddComponent<Component::Health>()->removeOnLowHealth = false;
    //body->AddComponent<Component::Spawner>()->delay = 0.05f;
    //body->AddComponent<Component::Animation>();

    /*Component::Animation::AnimationClip* idleBody = body->GetComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleBody->CreateKeyFrame(glm::vec3(0.1, 0, 0), 2, 1, 0, 1.5, false, true);
    idleBody->CreateKeyFrame(glm::vec3(0, 0.1, 0.1), -1, -2, 1, 1.5, false, true);
    idleBody->CreateKeyFrame(glm::vec3(0.1, 0.1, -0.1), -2, 1, 2, 1.5, false, true);
    idleBody->CreateKeyFrame(glm::vec3(-0.1, 0.1, 0.1), 2, 2, 1, 1.5, false, true);

    Component::Animation::AnimationClip* fireBody = body->GetComponent<Component::Animation>()->CreateAnimationClip("fire");
    fireBody->CreateKeyFrame(glm::vec3(0, 0, 0.4), 0, -10, 0, 0.1, false, true);
    fireBody->CreateKeyFrame(glm::vec3(0, 0, 0), 0, 0, 0, 0.2, false, true);

    body->GetComponent<Component::Animation>()->Start("idle");

    Entity* spotLight = mScene->CreateEntity();
    mEntityMap["spotLight"] = spotLight;
    spotLight->AddComponent<Component::RelativeTransform>()->Move(0, 1, 0);
    spotLight->GetComponent<Component::RelativeTransform>()->parentEntity = body;
    spotLight->GetComponent<Component::RelativeTransform>()->scale = glm::vec3(0.3f, 0.3f, 0.3f);
    spotLight->AddComponent<Component::Mesh>()->geometry = body->GetComponent<Component::Mesh>()->geometry;
    spotLight->AddComponent<Component::Material>();
    spotLight->AddComponent<Component::SpotLight>()->coneAngle = 90;
    spotLight->GetComponent<Component::SpotLight>()->attenuation = 0.1f;
    spotLight->AddComponent<Component::Physics>();
    spotLight->AddComponent<Component::Controller>();
    mEntityVector.push_back(spotLight);

    Entity* radar = mScene->CreateEntity();
    mEntityMap["radar"] = radar;
    radar->AddComponent<Component::RelativeTransform>()->parentEntity = body;
    radar->GetComponent<Component::Transform>()->scale = glm::vec3(0.5f, 0.5f, 0.5f);
    radar->AddComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    radar->AddComponent<Component::Material>();
    radar->AddComponent<Component::PointLight>()->attenuation = 0.5f;
    Component::Animation::AnimationClip* idleRadar = radar->AddComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleRadar->CreateKeyFrame(glm::vec3(3, 0, 0), 0, 0, 0, 1, true, false);
    idleRadar->CreateKeyFrame(glm::vec3(0, 0, 3), 360, 0, 0, 1, true, false);
    idleRadar->CreateKeyFrame(glm::vec3(-3, 0, 0), 720, 0, 0, 1, true, false);
    idleRadar->CreateKeyFrame(glm::vec3(0, 0, -3), 360, 0, 0, 1, true, false);
    radar->GetComponent<Component::Animation>()->Start("idle");
    mEntityVector.push_back(radar);*/
}

Player::~Player() {
    Resources().FreeCube();
    Resources().FreeCube();
    Resources().FreeCube();
}
