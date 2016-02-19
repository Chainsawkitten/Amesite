#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

#include <glm/glm.hpp>

class Scene;
class Entity;

namespace GameObject {
    /// Explosion
    class Explosion : public SuperGameObject {
        public:
            /// Create %Explosion
            /**
             * @param scene Pointer to which Scene %Explosion Enities' are contained.
             */
            Explosion(Scene* scene);
           
            ~Explosion();

            /// Node of the %Explosion.
            Entity* node;
    };
}
