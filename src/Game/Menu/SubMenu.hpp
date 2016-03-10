#pragma once

#include <glm/glm.hpp>
#include <vector>

class MenuOption;
class Font;

/// A sub menu inside a menu.
class SubMenu {
    public:
        /// Create new sub menu.
        SubMenu();
        
        /// Destructor.
        ~SubMenu();
        
        /// Set the menu's position.
        void SetPosition(const glm::vec3& position);
        
        /// Set the menu's rotation.
        void SetRotation(const glm::vec3& rotation);
        
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
};
