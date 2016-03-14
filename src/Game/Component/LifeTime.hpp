#pragma once

#include <Component/SuperComponent.hpp>

namespace Component {
    /// %Component to entites with a lifetime.
    class LifeTime : public SuperComponent {
        public:
            /// Create %LifeTime.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            LifeTime(Entity* entity);
            
            /// Destructor.
            ~LifeTime();
            
            /// How much time the Entity, to which this component corresponds, has left to live. (in seconds)
            /**
             * Default: 10.f
             */
            float lifeTime;

            /// How much time the Entity, to which this component corresponds, had at the beginning of it's life time (in seconds)
            /**
             * Default: lifeTime (10.f)
             */
            float initialLifeTime;
    };
}
