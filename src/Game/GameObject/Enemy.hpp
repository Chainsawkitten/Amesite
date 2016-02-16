#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;

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

            /// Body of %Enemy
            Entity* body;
    };
}
