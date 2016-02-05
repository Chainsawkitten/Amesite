#pragma once

class Entity;
namespace Component {
    class Controller;
}

namespace ControlScheme {
    /// Moves the entity with the left stick.
    /**
     * @param controller Component to control.
     * @param deltaTime Time since last frame (in seconds).
     */
    void Move(Component::Controller* controller, float deltaTime);
    
    /// Rotates the entity with the right stick.
    /**
     * @param controller Component to control.
     * @param deltaTime Time since last frame (in seconds).
     */
    void StickRotate(Component::Controller* controller, float deltaTime);
    
    /// Move the entity with the keyboard.
    /**
     * @param controller Component to control.
     * @param deltaTime Time since last frame (in seconds).
     */
    void ArrowKeyRotate(Component::Controller* controller, float deltaTime);
    
    /// Use a button to shoot.
    /**
     * @param controller Component to control.
     * @param deltaTime Time since last frame (in seconds).
     */
    void ButtonShoot(Component::Controller* controller, float deltaTime);
    
    /// Constantly fires bullets.
    /**
     * @param controller Component to control.
     * @param deltaTime Time since last frame (in seconds).
     */
    void AlwaysShoot(Component::Controller* controller, float deltaTime);
}
