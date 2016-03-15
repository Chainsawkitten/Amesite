#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>
#include <vector>

namespace GameObject {
    class SuperPlayer;
    class Camera;
    class SuperBoss;
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
             * @param players Vector of SuperPlayers which the camera will be relative to.
             * @param bosses Vector of SuperBosses which the camera will be relative to.
             * @param deltaTime Time since last frame (in seconds).
             */
            void UpdateRelativePosition(const std::vector<GameObject::SuperPlayer*>& players, const std::vector<GameObject::SuperBoss*>& bosses, float deltaTime);

            /// Body of camera
            Entity* body;
    };
}
