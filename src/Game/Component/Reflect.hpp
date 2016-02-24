#pragma once

#include <Component/SuperComponent.hpp>

namespace Component {
    /// %Component to entites that reflect all it collides with.
    class Reflect : public SuperComponent {
        public:
            /// Create %Reflect.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Reflect(Entity* entity);
            
            /// Destructor.
            ~Reflect();
    };
}
