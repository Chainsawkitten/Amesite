#pragma once

#include "SubMenu.hpp"

class Menu;

/// Main menu.
class MainMenu : public SubMenu {
    public:
        /// Create new main menu.
        /**
         * @param parentMenu %Menu in which this submenu is contained.
         */
        MainMenu(Menu* parentMenu);
        
        /// Get position the camera should have (relative to the menu).
        /**
         * @return Position of the camera relative to the menu.
         */
        glm::vec3 GetCameraPosition() const;
        
    private:
        void Quit();
};
