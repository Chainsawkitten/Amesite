#pragma once

#include <Engine/Component/SuperComponent.hpp>
#include <Engine/Util/Input.hpp>
#include <vector>

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
            /**
             * Default: InputHandler::PLAYER_ONE
             */
            InputHandler::Player playerID;
            
            /// Which device is controlling the entity.
            /**
             * Default: InputHandler::NONE
             */
            InputHandler::Device device;

            /// The speed of the controller
            /**
             * Default: 100.f
             */
            float speed;

            ///If the controller isn't enabled you don't use any control schemes
            bool enabled;

            /// Stores the control scheme functions for this controller.
            std::vector<void (*)(Controller* controller, float deltaTime)> controlSchemes;

    };
}
