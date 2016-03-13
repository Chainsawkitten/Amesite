#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>
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

            /// Body of camera
            Entity* body;

            /// Whether the camera is auto controlled
            /**
             * Default: false
             */
            bool autoControll;

        private:
            void mUpdateFunction();
    };
}
