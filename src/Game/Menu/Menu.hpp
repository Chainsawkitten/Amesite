#pragma once

#include <vector>
#include <glm/glm.hpp>

class Font;
class ShaderProgram;
namespace Geometry {
    class Plane;
}
class SubMenu;
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
        
        /// Pause game and enter the pause menu.
        void PauseGame();
        
        /// Pan out camera and resume game.
        void ResumeGame();
        
        /// Transition from current submenu to another.
        /**
         * @param subMenuIndex Index of the sub menu to transition to.
         */
        void Transition(int subMenuIndex);
        
    private:
        bool mActive;
        bool mFlyOut;
        bool mFlyIn;
        float mTimer;
        
        glm::vec3 mPosition;
        glm::vec3 mRotation;
        glm::mat4 mModelMatrix;
        
        // Transition between submenus.
        bool mTransition;
        int mNextSubMenu;
        float mTransitionTimer;
        
        // Text.
        Font* mFont;
        
        // Sub menus.
        std::vector<SubMenu*> mSubMenus;
        int mSelected;
};
