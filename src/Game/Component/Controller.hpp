#pragma once

#include <Engine\Component\SuperComponent.hpp>
#include <Util/Input.hpp>

class InputHandler;

namespace Component {
    ///Component providing player control.
    class Controller : public SuperComponent {

        public:

            /// Create new %Player.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Controller(Entity* entity);

            /// Destructor.
            ~Controller();

            InputHandler::Player playerID;
            void(*ControlScheme)(Controller* controller, float deltaTime);

    };

}
