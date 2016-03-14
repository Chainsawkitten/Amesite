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
    body->GetComponent<Component::Controller>()->controlSchemes.push_back(ControlScheme::CameraChangeControl);
    body->GetComponent<Component::Controller>()->enabled = false;
    body->GetComponent<Component::Controller>()->playerID = InputHandler::ANYONE;
    body->AddComponent<Component::Update>()->updateFunction = std::bind(&Camera::mUpdateFunction, this);
    state = CameraState::AUTO;
    mLastState = state;
}

Camera::~Camera() {
}

void Camera::mUpdateFunction() {
    if (autoControll)
        body->GetComponent<Component::Controller>()->enabled = true;
    else 
        body->GetComponent<Component::Controller>()->enabled = false;

    if (mLastState != state) {
        std::vector<void(*)(Component::Controller* controller, float deltaTime)>* vec = &body->GetComponent<Component::Controller>()->controlSchemes;
        if (mLastState == CameraState::AUTO) {
            // remove auto
            vec->erase(std::remove(vec->begin(), vec->end(), ControlScheme::CameraAuto), vec->end());
        } else {
            // remove free
        }
        if (state == CameraState::AUTO) {
            // Add auto
            body->GetComponent<Component::Controller>()->controlSchemes.push_back(ControlScheme::CameraAuto);
        } else {
            // Add free
        }
        mLastState = state;
    }
}
