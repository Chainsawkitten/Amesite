#pragma once

#include <Scene/Scene.hpp>
#include <System/RenderSystem.hpp>
#include <functional>

class PostProcessing;
class FXAAFilter;
class GammaCorrectionFilter;
class GlowFilter;
class GlowBlurFilter;
namespace GameObject {
    class Camera;
}
class Font;
class Texture2D;
namespace Geometry {
    class Plane;
}
class ShaderProgram;

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
        
        // The main camera
        GameObject::Camera* mMainCamera;
        
        // Post processing.
        PostProcessing* mPostProcessing;
        FXAAFilter* mFxaaFilter;
        GammaCorrectionFilter* mGammaCorrectionFilter;
        GlowFilter* mGlowFilter;
        GlowBlurFilter* mGlowBlurFilter;
        
        // Text.
        Font* mFont;
        
        // Used to render 3D text.
        Geometry::Plane* mPlane;
        ShaderProgram* mTextShaderProgram;
        ShaderProgram* mSelectedShaderProgram;
        
        // A menu option.
        class MenuOption {
            public:
                Texture2D* prerenderedText;
                glm::vec3 position;
                glm::vec3 rotation;
                glm::vec2 scale;
                
                std::function<void()> callback;
                
                MenuOption(Font* font, const char* text, const glm::vec3& position, const glm::vec3& rotation, float height);
                ~MenuOption();
                
                glm::mat4 GetModelMatrix() const;
                
                void EmptyCallback() const;
        };
        std::vector<MenuOption*> mMenuOptions;
        
        int mSelected;
        
        void RenderSelectedMenuOption(const MenuOption* menuOption, const glm::vec2& screenSize);
        void RenderMenuOption(const MenuOption* menuOption, const glm::vec2& screenSize);
};
