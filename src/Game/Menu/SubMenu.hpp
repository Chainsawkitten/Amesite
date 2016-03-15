#pragma once

#include <glm/glm.hpp>
#include <vector>

class SuperMenuOption;
class MenuOption;
class Font;

/// A sub menu inside a menu.
class SubMenu {
    public:
        /// Create new sub menu.
        SubMenu();
        
        /// Destructor.
        virtual ~SubMenu();
        
        /// Set the menu's position.
        /**
         * @param position New position.
         */
        void SetPosition(const glm::vec3& position);
        
        /// Get the menu's position.
        /**
         * @return The menu's position (relative to it's parent menu).
         */
        const glm::vec3& GetPosition() const;
        
        /// Set the menu's rotation.
        /**
         * @param rotation New rotation.
         */
        void SetRotation(const glm::vec3& rotation);
        
        /// Update model matrix.
        /**
         * @param menuModelMatrix Model matrix of the parent menu.
         */
        void UpdateModelMatrix(const glm::mat4& menuModelMatrix);
        
        /// Update the menu.
        /**
         * @param playerScale Scale of the player.
         */
        void Update(const glm::vec2& playerScale);
        
        /// Render selected menu option.
        void RenderSelected();
        
        /// Render menu options.
        void RenderMenuOptions();
        
        /// Get position the camera should have (relative to the menu).
        /**
         * @return Position of the camera relative to the menu.
         */
        virtual glm::vec3 GetCameraPosition() const = 0;
        
        /// Get direction the camera should have.
        /**
         * @return Direction of the camera.
         */
        virtual glm::vec3 GetCameraDirection() const = 0;
        
    protected:
        /// Add a menu option to the sub menu.
        /**
         * @param menuOption %Menu option to add.
         */
        void AddMenuOption(SuperMenuOption* menuOption);
        
        /// Set title option.
        /**
         * Sets a menu option to display as the title of the menu.
         * This menu option can't be interacted with.
         * @param titleOption %Menu option to display as title.
         */
        void SetTitleOption(MenuOption* titleOption);
        
        /// Get the menu's font.
        /**
         * @return The menu's font.
         */
        Font* GetFont() const;
        
    private:
        glm::vec3 mPosition;
        glm::vec3 mRotation;
        glm::mat4 mModelMatrix;
        
        // Text.
        Font* mFont;
        
        // Menu options.
        std::vector<SuperMenuOption*> mMenuOptions;
        int mSelected;
        
        MenuOption* mTitleOption;
};
