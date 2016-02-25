#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>
#include "../GameObject/Enemy.hpp"

class Scene;
class Entity;

namespace GameObject {
    class Enemy;
}


namespace GameObject {
    /// A Enemy
    class EnemySpawner : public SuperGameObject {
    public:
        /// Create %Enemy
        /**
        * @param scene Pointer to which Scene %Enemy Enities' are contained.
        */
        EnemySpawner(Scene* scene, Enemy::EnemyType type);

        ~EnemySpawner();

        /// Body of spawner.
        Entity* body;

    private:
        // Type of enemies to spawn.
        Enemy::EnemyType mType;
    };
}
