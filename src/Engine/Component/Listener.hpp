#pragma once

#include "SuperComponent.hpp"

class Entity;

namespace Component {
    /// %Component describing a listener that can listen to sounds.
    class Listener : public SuperComponent {
        public:
            /// Create new listener.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Listener(Entity* entity);
    };
}
