#pragma once

#include <Engine/Component/SuperComponent.hpp>
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

            /// Stores the control scheme functions for this controller.
            std::vector<void (*)(Controller* controller, float deltaTime)> controlSchemes;
    };
}
