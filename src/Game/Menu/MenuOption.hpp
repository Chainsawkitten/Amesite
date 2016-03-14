#pragma once

#include "SuperMenuOption.hpp"
#include <functional>

class Texture2D;
class Font;
namespace Geometry {
    class Plane;
}
class ShaderProgram;

/// An option in a menu.
class MenuOption : SuperMenuOption {
    public:
        /// Function to call when pressed.
        std::function<void()> callback;
        
        /// Create new menu option.
        /**
         * @param font %Font to use to render the text.
         * @param text Text to display.
         * @param position Position of the menu option (relative in the menu).
         * @param rotation Rotation of the menu option (relative to the menu).
         * @param height How tall the menu option should be (width will be scaled accordingly).
         */
        MenuOption(Font* font, const char* text, const glm::vec3& position, const glm::vec3& rotation, float height);
        
        /// Destructor.
        ~MenuOption();
        
        /// Get the scale of the menu option.
        /**
         * @return Scale of the menu option.
         */
        glm::vec2 GetScale() const;
        
        /// Get whether the mouse cursor intersects the menu option.
        /**
         * @param cameraPosition The main camera's position.
         * @param ray The mouse ray as projected in the world.
         * @param menuModelMatrix Model matrix of the parent menu.
         * @param playerScale Scale of the player model.
         * @return Whether the mouse cursor intersects the menu option.
         */
        bool MouseIntersect(const glm::vec3& cameraPosition, const glm::vec3& ray, const glm::mat4& menuModelMatrix, const glm::vec2& playerScale);
        
        /// Render selection.
        /**
         * Render a black square behind the text of the menu option.
         * @param screenSize Size of the context to render to.
         * @param menuModelMatrix Model matrix of the parent menu.
         */
        void RenderSelected(const glm::vec2& screenSize, const glm::mat4& menuModelMatrix);
        
        /// Render the menu option.
        /**
         * @param screenSize Size of the context to render to.
         * @param menuModelMatrix Model matrix of the parent menu.
         */
        void Render(const glm::vec2& screenSize, const glm::mat4& menuModelMatrix);
        
    private:
        // Used to render 3D text.
        Geometry::Plane* mPlane;
        ShaderProgram* mTextShaderProgram;
        ShaderProgram* mSelectedShaderProgram;
        
        Texture2D* mPrerenderedText;
        glm::vec3 mPosition;
        glm::vec3 mRotation;
        glm::vec2 mScale;
        
        void EmptyCallback() const;
};
