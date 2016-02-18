#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;

namespace GameObject {
    /// A bullet
    class Bullet : public SuperGameObject {
        public:
            /// Create %Bullet
            /**
             * @param scene Pointer to which Scene %Bullet Enities' are contained.
             */
            Bullet(Scene* scene);
           
            ~Bullet();

            /// Body of the %Bullet
            Entity* body;

            Entity* fireEmitter;

            Entity* blueEmitter;
    };
}
