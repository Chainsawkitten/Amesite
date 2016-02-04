#pragma once

#include <../Game/Component/Controller.hpp>

class Entity;

namespace ControlScheme {

    void StickMove(Component::Controller* controller, float deltaTime);
    void StickRotate(Component::Controller* controller, float deltaTime);
    void ArrowKeyRotate(Component::Controller* controller, float deltaTime);
    void ArrowKeysMove(Component::Controller* controller, float deltaTime);

}