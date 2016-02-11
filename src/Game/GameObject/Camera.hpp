#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;

namespace GameObject {
    /// A Camera
    class Camera : public SuperGameObject {
        public:
            /// Create %Camera
            /**
             * @param scene Pointer to which Scene %Camera Enities' are contained.
             */
            Camera(Scene* scene);
           
            ~Camera();
    };
}
