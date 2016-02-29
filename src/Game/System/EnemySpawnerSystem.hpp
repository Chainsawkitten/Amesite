#pragma once

#include <glm\glm.hpp>
#include <vector>

class Scene;

namespace GameObject {
    class Enemy;
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
            */
            void Update(Scene& scene, float deltaTime, GameObject::Cave* cave);
        private:
            // Enemy count.
            unsigned int mMaxEnemyCount;
            unsigned int mEnemyCount;

            // Retrieves a valid random spawn position from the map.
            glm::vec3 FindValidPosition(GameObject::Cave* cave);

            // Keeps track of the active enemies in the scene.
            std::vector<GameObject::Enemy*> mEnemies;

    };
}
