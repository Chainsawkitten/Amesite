#pragma once

#include <Component/SuperComponent.hpp>

namespace Component {
    /// %Component spawning other entities.
    class Spawner : public SuperComponent {
        public:
            /// Different types of spawners
            enum SpawnerType {
                ENEMY = 0,
                BULLET,
            };

            /// Different types of enemy spawners.
            enum EnemyType {
                BASIC = 0,
                PYLON,
                ENEMIES,
            };

            /// Create new spawner.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Spawner(Entity* entity);
            
            /// Destructor.
            ~Spawner();
            
            /// Elapsed time since last spawn (in second).
            float timeSinceSpawn;
            /**
             * Default: 0.f
             */
            
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

            /// SpawnerType, type of spawner.
            /**
             * Default: BULLET (1)
             */
            SpawnerType type;

            /// enemyType, type of enemy to spawn (if it is an enemy spawner).
            /**
            * Default: BASIC (0)
            */
            EnemyType enemyType;
    };
}
