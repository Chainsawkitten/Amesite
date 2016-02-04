#pragma once

#include "CameraSystem.hpp"

class Scene;
class Entity;

namespace System {
    class CameraSystem {
        public:
            CameraSystem();
            ~CameraSystem();
            void Update(Scene& scene, float deltaTime);
        private:

    };
}