#pragma once

#include <vector>

class MenuOption;

/// A sub menu inside a menu.
class SubMenu {
    public:
        /// Create new sub menu.
        SubMenu();
        
        /// Destructor.
        ~SubMenu();
        
    private:
        // Menu options.
        std::vector<MenuOption*> mMenuOptions;
        int mSelected;
};
