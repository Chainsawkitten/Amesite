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
            /**
             * Default: 0.f
             */
            float timeSinceSpawn;
            
            /// Delay between spawns.
            /**
             * Default: 0.f
             */
            float delay;

            /// Faction, ID of the faction this spawner belongs to (players, enemies).
            /**
             * Default: 0
             */
            int faction;
    };
}
