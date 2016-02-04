#pragma once

#include <Engine/Component/SuperComponent.hpp>
#include <Util/Input.hpp>

namespace Component {
    ///Component providing control.
    class Controller : public SuperComponent {

        public:

            /// Create new %Controller.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Controller(Entity* entity);

            /// Destructor.
            ~Controller();
            
            /// Id for controll
            /**
             * Default: InputHandler::PLAYER_ONE
             */
            InputHandler::Player playerID;

    };

}
