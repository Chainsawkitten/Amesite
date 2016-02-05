#pragma once

class Entity;
namespace Component {
    class Controller;
}

namespace ControlScheme {
    /// Moves the entity with the left stick.
    void Move(Component::Controller* controller, float deltaTime);
    
    /// Rotates the entity with the right stick.
    void StickRotate(Component::Controller* controller, float deltaTime);
    
    /// Move the entity with the keyboard.
    void ArrowKeyRotate(Component::Controller* controller, float deltaTime);
}
