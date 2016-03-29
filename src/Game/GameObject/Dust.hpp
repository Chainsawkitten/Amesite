#pragma once

#include <GameObject/SuperGameObject.hpp>

class Scene;
class Entity;

namespace GameObject {
    /// A Camera
    class Dust : public SuperGameObject {
        public:
            /// Create %Bullet
            /**
             * @param scene Pointer to which Scene %Dust Enities' are contained.
             */
            Dust(Scene* scene);

            ~Dust();

            /// Body of the dust
            Entity* body;
    };
}
