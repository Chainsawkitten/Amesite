#pragma once

#include "SubMenu.hpp"
#include <string>

class Menu;

/// Pause menu.
class PauseMenu : public SubMenu {
    public:
        /// Create new pause menu.
        /**
         * @param parentMenu %Menu in which this submenu is contained.
         */
        PauseMenu(Menu* parentMenu);
        
        /// Get position the camera should have (relative to the menu).
        /**
         * @return Position of the camera relative to the menu.
         */
        glm::vec3 GetCameraPosition() const;
        
        /// Get direction the camera should have.
        /**
         * @return Direction of the camera.
         */
        glm::vec3 GetCameraDirection() const;
        
    private:
        void Quit();
};
