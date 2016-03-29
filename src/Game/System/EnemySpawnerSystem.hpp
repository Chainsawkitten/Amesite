#pragma once

#include <glm\glm.hpp>
#include <vector>
#include <random>

class Scene;

namespace GameObject {
    class SuperPlayer;
    class SuperBoss;
    class SuperEnemy;
    class EnemySpawner;
    class Cave;
}

namespace System {
    /// Class that handles spawning enemies at different positions around the map.
    class EnemySpawnerSystem {
        public:
            /// Create the spawner system.
            EnemySpawnerSystem();

            /// Destructor.
            ~EnemySpawnerSystem();

            /// Update Scene containing entities. Updates AI components.
            /**
             * @param scene The scene to update.
             * @param deltaTime Time since last frame (in seconds).
             * @param cave Cave from which to access data about terrain.
             * @param players List of players to access their positions.
             * @param noSpawnRooms List of rooms in which no enemies should spawn.
             */
            void Update(Scene& scene, float deltaTime, const GameObject::Cave* cave, const std::vector<glm::vec3> noSpawnRooms);

            ///Returns amount of enemies killed.
            /**
             * @return The amount of enemies killed.
             */
            unsigned int GetEnemiesKilled();
            
            /// Get list of all enemies.
            /**
             * @return All enemies.
             */
            const std::vector<GameObject::SuperEnemy*>& GetEnemies() const;

        private:
            // Random Engine
            std::mt19937 mRNG;

            // Enemy count.
            unsigned int mMaxEnemyCount;
            unsigned int mMinEnemyCount;
            unsigned int mEnemyCount;
            unsigned int mEnemiesKilled;
            float mSpawnerRadius;
            glm::vec3 mMapScale;

            // Retrieves a valid random spawn position from the map.
            glm::vec3 FindValidPosition(const GameObject::Cave* cave, const std::vector<glm::vec3> noSpawnRooms);

            // Check if far away enough from all bosses.
            bool ProximityToBosses(const glm::vec3 position, const std::vector<glm::vec3> noSpawnRooms, int bossRadius) const;

            // Keeps track of the active enemies in the scene.
            std::vector<GameObject::SuperEnemy*> mEnemies;

    };
}
