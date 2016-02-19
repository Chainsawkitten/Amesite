#pragma once

class Entity;
namespace Component {
    class Controller;
}

namespace ControlScheme {

    /// A scheme that doesn't do anything
    void Empty(Component::Controller* controller, float deltaTime);

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

    /// Rotate the entity with the keyboard.
    void ArrowKeysMove(Component::Controller* controller, float deltaTime);

    /// Rotates the object
    void AutoRotate(Component::Controller* controller, float deltaTime);

    /// Moves the object in a random direction
    void RandomMove(Component::Controller* controller, float deltaTime);

    /// Constantly fires bullets.
    /**
     * @param controller Component to control.
     * @param deltaTime Time since last frame (in seconds).
     */
    void AlwaysShoot(Component::Controller* controller, float deltaTime);
    
    /// Rotates the enitity with the AIM input.
    /**
     * @param controller Component to control.
     * @param deltaTime Time since last frame (in seconds).
     */
    void Aim(Component::Controller* controller, float deltaTime);
    
    /// Rotates the enitity with the mouse input.
    /**
     * @param controller Component to control.
     * @param deltaTime Time since last frame (in seconds).
     */
    void MouseAim(Component::Controller* controller, float deltaTime);

    /// Shoots in the direction the object is facing.
    /**
     * @param controller Component to control.
     * @param deltaTime Time since last frame (in seconds).
     */
    void AimedFire(Component::Controller* controller, float deltaTime);

    /// Mouse rotation.
    /**
     * @param controller Component to control.
     * @param deltaTime Time since last frame (in seconds).
     */
    void MouseRotate(Component::Controller* controller, float deltaTime);

}
