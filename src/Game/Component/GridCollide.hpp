#pragma once

#include <Component/SuperComponent.hpp>

namespace Component {
    /// %Component checking collision with cave
    class GridCollide : public SuperComponent {
        public:
            /// Create %GridCollide.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            GridCollide(Entity* entity);
            
            /// Destructor.
            ~GridCollide();
    };
}
