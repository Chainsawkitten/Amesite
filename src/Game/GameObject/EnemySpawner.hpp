#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;


namespace GameObject {
    /// A Enemy
    class EnemySpawner : public SuperGameObject {
    public:
        /// Create %Enemy
        /**
        * @param scene Pointer to which Scene %Enemy Enities' are contained.
        */
        EnemySpawner(Scene* scene, EnemyType type);

        ~EnemySpawner();

        /// Body of spawner.
        Entity* body;

    private:
        // Type of enemies to spawn.
        EnemyType mType;
    };
}
