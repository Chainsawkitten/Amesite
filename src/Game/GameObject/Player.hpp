#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;

namespace GameObject {
    /// A Player
    class Player : public SuperGameObject {
        public:
            /// Create %Player
            /**
             * @param scene Pointer to which Scene %Player Enities' are contained.
             */
            Player(Scene* scene);
           
            ~Player();
    };
}
