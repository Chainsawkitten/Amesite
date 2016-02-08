#pragma once

#include <Component/SuperComponent.hpp>
#include <Util/Input.hpp>
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
            InputHandler::Player playerID;
            
            /// The speed of the controller
            float mSpeed;

            /// Stores the control scheme functions for this controller.
            std::vector<void (*)(Controller* controller, float deltaTime)> controlSchemes;

    };
}
