#pragma once

#include <Scene/Scene.hpp>
#include <vector>

class Texture2D;

/// Splash screen shown when starting the game.
/**
 * Shows logos and then continues to main scene.
 */
class SplashScene : public Scene {
    public:
        /// Create splash scene.
        SplashScene();
        
        /// Destructor
        ~SplashScene();
        
        /// Update the scene.
        /**
         *@param deltaTime Time since last frame.
         */
        void Update(float deltaTime);
        
    private:
        Texture2D* mLogo;
};
