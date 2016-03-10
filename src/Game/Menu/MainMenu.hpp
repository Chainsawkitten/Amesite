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
        
    private:
        void Quit();
};
