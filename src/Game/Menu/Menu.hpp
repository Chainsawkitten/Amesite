#pragma once

#include <vector>
#include <glm/glm.hpp>

class Font;
class ShaderProgram;
namespace Geometry {
    class Plane;
}
class MenuOption;
namespace GameObject {
    class SuperPlayer;
}

/// An interactive menu showing different submenus.
class Menu {
    public:
        /// Create new menu.
        Menu();
        
        /// Destructor.
        ~Menu();
        
        /// Get whether the menu is active.
        /**
         * @return Whether the player is currently in the menu.
         */
        bool IsActive() const;
        
        /// Set the menu's position.
        void SetPosition(const glm::vec3& position);
        
        /// Set the menu's rotation.
        void SetRotation(const glm::vec3& rotation);
        
        /// Update the menu.
        /**
         * @param player Player around which to have the menu.
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(GameObject::SuperPlayer* player, float deltaTime);
        
        /// Render selected menu option.
        void RenderSelected();
        
        /// Render menu options.
        void RenderMenuOptions();
        
    private:
        bool mActive;
        bool mFlyOut;
        float mTimer;
        
        glm::vec3 mPosition;
        glm::vec3 mRotation;
        glm::mat4 mModelMatrix;
        
        // Text.
        Font* mFont;
        
        // Used to render 3D text.
        Geometry::Plane* mPlane;
        ShaderProgram* mTextShaderProgram;
        ShaderProgram* mSelectedShaderProgram;
        
        // Menu options.
        std::vector<MenuOption*> mMenuOptions;
        int mSelected;
        
        void RenderSelectedMenuOption(const MenuOption* menuOption, const glm::vec2& screenSize, const glm::mat4& menuModelMatrix);
        void RenderMenuOption(const MenuOption* menuOption, const glm::vec2& screenSize, const glm::mat4& menuModelMatrix);
        
        void StartGame();
        void Quit();
};
