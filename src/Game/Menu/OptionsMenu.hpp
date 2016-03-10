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
};
