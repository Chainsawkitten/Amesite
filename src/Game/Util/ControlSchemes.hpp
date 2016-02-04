#pragma once

#include <../Game/Component/Controller.hpp>

class Entity;

namespace ControlScheme {

    /// Moves the entity with the left stick.
    void StickMove(Component::Controller* controller, float deltaTime);
    
    /// Rotates the entity with the right stick.
    void StickRotate(Component::Controller* controller, float deltaTime);
    
    /// Move the entity with the keyboard.
    void ArrowKeyRotate(Component::Controller* controller, float deltaTime);
    
    /// Rotate the entity with the keyboard.
    void ArrowKeysMove(Component::Controller* controller, float deltaTime);

}