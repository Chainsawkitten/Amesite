#pragma once

#include <Scene/Scene.hpp>
#include <System/RenderSystem.hpp>

class Texture2D;
class PostProcessing;
class FXAAFilter;
class GammaCorrectionFilter;
class GlowFilter;
class GlowBlurFilter;

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
        // RenderSystem.
        System::RenderSystem mRenderSystem;
        
        // Post processing.
        PostProcessing* postProcessing;
        FXAAFilter* fxaaFilter;
        GammaCorrectionFilter* gammaCorrectionFilter;
        GlowFilter* glowFilter;
        GlowBlurFilter* glowBlurFilter;
        
        Texture2D* mLogo;
};
