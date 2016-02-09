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
             * @param scene Pointer to which Scene %Bullet Enitie's are contained.
             */
            Bullet(Scene* scene);
           
            ~Bullet();
    };
}
