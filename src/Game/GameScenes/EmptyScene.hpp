#pragma once

#include <Scene/Scene.hpp>

/// Empty scene that does nothing
class EmptyScene : public Scene {
    public:
        /// Update the scene.
        /**
        *@param deltaTime Time since last frame.
        */
        void Update(float deltaTime);

        ///Constructor
        EmptyScene();

        ///Destructor
        ~EmptyScene();
};