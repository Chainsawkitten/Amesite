#pragma once

#include <Engine/Scene/Scene.hpp>

class Texture2D;

/// Loading screen showing a static image while loading the main scene.
class LoadingScene : public Scene {
    public:
        /// Create loading scene.
        LoadingScene();
        
        /// Destructor
        ~LoadingScene();
        
        /// Update the scene.
        /**
         *@param deltaTime Time since last frame.
         */
        void Update(float deltaTime);
        
    private:
        bool mShown;
        Texture2D* mLoadingImage;
};
