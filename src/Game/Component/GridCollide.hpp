#pragma once

#include <Engine/Component/SuperComponent.hpp>

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

            /// Remove Entity to which this %Component corresponds if true.
            /**
             * Default: true
             */
            bool removeOnImpact;

            /// True if object has collided with grid.
            /**
             * Default: false
             */
            bool hasCollided;
    };
}
