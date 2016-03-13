#pragma once

#include <Component/SuperComponent.hpp>

namespace Component {
    /// %Component checking collision with terrain
    class GroundCollide : public SuperComponent {
        public:
            /// Create %GroundCollide.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            GroundCollide(Entity* entity);
            
            /// Destructor.
            ~GroundCollide();

            /// Remove Entity to which this %Component corresponds if true.
            /**
             * Default: true
             */
            bool removeOnImpact;

            /// True if object has collided with terrain.
            /**
             * Default: false
             */
            bool hasCollided;
    };
}
