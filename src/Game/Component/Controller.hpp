#pragma once

#include <Engine/Component/SuperComponent.hpp>
#include <Util/Input.hpp>

namespace Component {
    /// %Component providing player control.
    class Controller : public SuperComponent {
        public:
            /// Create new %Controller.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Controller(Entity* entity);
            
            /// Destructor.
            ~Controller();
            
            /// Which player is controlling the entity.
            InputHandler::Player playerID;

            /// Stores the control scheme function for this controller.
            /**
             * @param The controller %Component.
             * @param The delta time of the frame.
             */
            void(*ControlScheme)(Controller* controller, float deltaTime);
            
            /// The speed of the controller
            float mSpeed;
    };
}
