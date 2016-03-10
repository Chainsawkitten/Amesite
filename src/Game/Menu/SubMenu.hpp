#pragma once

#include <glm/glm.hpp>
#include <vector>

class MenuOption;
class Menu;
class Font;

/// A sub menu inside a menu.
class SubMenu {
    public:
        /// Create new sub menu.
        /**
         * @param parentMenu %Menu in which this submenu is contained.
         */
        SubMenu(Menu* parentMenu);
        
        /// Destructor.
        ~SubMenu();
        
        /// Set the menu's position.
        void SetPosition(const glm::vec3& position);
        
        /// Set the menu's rotation.
        void SetRotation(const glm::vec3& rotation);
        
        /// Update the menu.
        /**
         * @param menuModelMatrix Model matrix of the parent menu.
         * @param playerScale Scale of the player.
         */
        void Update(const glm::mat4& menuModelMatrix, const glm::vec2& playerScale);
        
        /// Render selected menu option.
        void RenderSelected();
        
        /// Render menu options.
        void RenderMenuOptions();
        
    private:
        glm::vec3 mPosition;
        glm::vec3 mRotation;
        glm::mat4 mModelMatrix;
        
        // Text.
        Font* mFont;
        
        // Menu options.
        std::vector<MenuOption*> mMenuOptions;
        int mSelected;
        
        void Quit();
};
