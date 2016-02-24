#pragma once

#include <Scene/Scene.hpp>

class Texture2D;

/// Main menu.
class MenuScene : public Scene {
    public:
        /// Create menu scene.
        MenuScene();
        
        /// Destructor
        ~MenuScene();
        
        /// Update the scene.
        /**
         *@param deltaTime Time since last frame.
         */
        void Update(float deltaTime);
        
    private:
        Texture2D* mLogo;
};
