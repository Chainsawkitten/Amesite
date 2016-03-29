#pragma once

#include <GameObject/SuperGameObject.hpp>

class Scene;
class Entity;

namespace Component {
    class Spawner;
}


namespace GameObject {
    /// A Enemy
    class EnemySpawner : public SuperGameObject {
    public:
        /// Create %Enemy
        /**
         * @param scene Pointer to which Scene %Enemy Enities' are contained.
         * @param type of enemy to spawn - documented in Component::Spawner::EnemyType.
         */
        EnemySpawner(Scene* scene, unsigned int type);

        ~EnemySpawner();

        /// Body of spawner.
        Entity* body;
    };
}
