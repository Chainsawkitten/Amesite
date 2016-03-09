#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

#include <glm/glm.hpp>

class Scene;
class Entity;

namespace GameObject {
    /// A fading light
    class DyingLight : public SuperGameObject {
        public:
            /// Create %DyingLight
            /**
             * @param scene Pointer to which Scene %DyingLight Enities' are contained.
             */
            DyingLight(Scene* scene);
           
            ~DyingLight();

            /// Node of the %DyingLight.
            Entity* node;

            void mUpdateFunction();

    };
}
