#include "Camera.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Lens.hpp>
#include <Engine/Component/Listener.hpp>
#include <Engine/Component/Physics.hpp>
#include "../Component/Controller.hpp"
#include "../Component/Update.hpp"

#include "../GameObject/Player/SuperPlayer.hpp"
#include "../GameObject/Camera.hpp"

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

Camera::Camera(Scene* scene) : SuperGameObject(scene) {
    autoControll = false;
    body = CreateEntity();
    body->AddComponent<Component::Transform>();
    body->AddComponent<Component::Physics>()->maxVelocity = 10000.f;
    body->AddComponent<Component::Lens>();
    body->AddComponent<Component::Listener>();
    body->AddComponent<Component::Controller>()->controlSchemes.push_back(ControlScheme::CameraAuto);
    body->GetComponent<Component::Controller>()->enabled = false;
    body->AddComponent<Component::Update>()->updateFunction = std::bind(&Camera::mUpdateFunction, this);
}

Camera::~Camera() {
}

void Camera::mUpdateFunction() {
    if (autoControll)
        body->GetComponent<Component::Controller>()->enabled = true;
    else 
        body->GetComponent<Component::Controller>()->enabled = false;
}
