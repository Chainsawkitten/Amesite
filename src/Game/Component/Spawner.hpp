#pragma once

#include <Component/SuperComponent.hpp>

namespace Component {
    /// %Component spawning other entities.
    class Spawner : public SuperComponent {
        public:
            /// Create new spawner.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Spawner(Entity* entity);
            
            /// Destructor.
            ~Spawner();
            
            /// Elapsed time since last spawn (in second).
            float timeSinceSpawn;
            
            /// Delay between spawns.
            float delay;
    };
}
