#pragma once

#include <vector>
#include <glm/glm.hpp>

class Font;
class ShaderProgram;
namespace Geometry {
    class Plane;
}
class MenuOption;

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
        
        /// Update the menu.
        void Update();
        
        /// Render selected menu option.
        void RenderSelected();
        
        /// Render menu options.
        void RenderMenuOptions();
        
    private:
        // Text.
        Font* mFont;
        
        // Used to render 3D text.
        Geometry::Plane* mPlane;
        ShaderProgram* mTextShaderProgram;
        ShaderProgram* mSelectedShaderProgram;
        
        // Menu options.
        std::vector<MenuOption*> mMenuOptions;
        
        void RenderMenuOption(const MenuOption* menuOption, const glm::vec2& screenSize);
};
