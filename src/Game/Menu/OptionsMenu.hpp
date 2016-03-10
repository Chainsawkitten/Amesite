#pragma once

#include "SubMenu.hpp"

class Menu;

/// Options menu.
class OptionsMenu : public SubMenu {
    public:
        /// Create new options menu.
        /**
         * @param parentMenu %Menu in which this submenu is contained.
         */
        OptionsMenu(Menu* parentMenu);
        
        /// Get position the camera should have (relative to the menu).
        /**
         * @return Position of the camera relative to the menu.
         */
        glm::vec3 GetCameraPosition() const;
};
