#pragma once

#include "CameraSystem.hpp"

class Scene;
class Entity;

namespace System {
    class CameraSystem {
        public:
            ///Constructor
            CameraSystem();

            ///Destructor
            ~CameraSystem();

            ///Update camera system.
            /**
            * @param scene The scene to update.
            * @param deltaTime Time since last frame (in seconds).
            */
            void Update(Scene& scene, float deltaTime);
        private:

    };
}