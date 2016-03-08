#pragma once

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
        
        /// Update the menu.
        void Update();
        
        /// Render selected menu option.
        void RenderSelected();
        
        /// Render menu options.
        void RenderMenuOptions();
};
