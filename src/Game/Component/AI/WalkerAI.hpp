#pragma once

#include <Component/SuperComponent.hpp>
#include <Util/Input.hpp>
#include <vector>
#include <glm/glm.hpp>

namespace Component {
    /// %Component providing player control.
    class WalkerAI : public SuperComponent {
        public:
            /// Create new %Controller.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            WalkerAI(Entity* entity);
            
            /// Destructor.
            ~WalkerAI();

            ///Where to go.
            glm::vec3 target;

            /// Updates the walker AI
            /**
            * @param The delta time
            */
            void Update(float timeDelta);

    };
}
