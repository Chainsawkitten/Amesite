#pragma once

#include "SuperMenuOption.hpp"

class Font;

/// Switch for the options menu.
class MenuSwitch : public SuperMenuOption {
    public:
        /// Create new menu option.
        /**
         * @param font %Font to use to render the text.
         * @param text Text to display.
         * @param position Position of the menu option (relative in the menu).
         * @param rotation Rotation of the menu option (relative to the menu).
         * @param height How tall the menu option should be (width will be scaled accordingly).
         */
        MenuSwitch(Font* font, const char* text, const glm::vec3& position, const glm::vec3& rotation, float height);
        
        /// Destructor.
        ~MenuSwitch();
        
        /// Get the scale of the menu option.
        /**
         * @return Scale of the menu option.
         */
        glm::vec2 GetScale() const;
        
        /// Get prerendered text texture.
        /**
         * @return The prerendered texture.
         */
        Texture2D* GetTexture() const;
        
        /// Press the menu option.
        void Press();
};
