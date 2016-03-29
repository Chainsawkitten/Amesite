#pragma once

#include "SubMenu.hpp"
#include <string>

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
        
        /// Get direction the camera should have.
        /**
         * @return Direction of the camera.
         */
        glm::vec3 GetCameraDirection() const;
        
    private:
        void Fullscreen(std::string option);
        void FXAA(std::string option);
        void TwoPlayers(std::string option);
        void MouseAim(std::string option);
        void WaterReflections(std::string option);
        void WaterRefractions(std::string option);
        void AudioVolume(std::string option);
};
