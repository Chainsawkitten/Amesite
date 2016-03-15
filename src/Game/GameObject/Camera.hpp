#pragma once

#include <GameObject/SuperGameObject.hpp>
#include <vector>

namespace GameObject {
    class SuperPlayer;
    class Camera;
}

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

            /// Update position of the camera
            /**
             * @param players Vector of SuperPlayer's which the camera will be relative to.
             */
            void UpdateRelativePosition(const std::vector<GameObject::SuperPlayer*>& players);

            /// Body of camera
            Entity* body;
    };
}
