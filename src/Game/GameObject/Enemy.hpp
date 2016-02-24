#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// A Enemy
    class Enemy : public SuperGameObject {
        public:
            /// Create %Enemy
            /**
             * @param scene Pointer to which Scene %Enemy Enities' are contained.
             */
            Enemy(Scene* scene);
           
            ~Enemy();

            /// Node of %Enemy
            Entity* node;
            /// Head relative to Node
            Entity* head;
            /// Tail relative to Node
            Entity* tail;
            /// Turrent relative to Head
            Entity* turret;

            /// Gets health of the enemy.
            /**
             * @return health of the enemy.
             */
            float GetHealth();

        private:
            Geometry::OBJModel* mEnemyHead;
            Geometry::OBJModel* mEnemyTail;
    };
}
