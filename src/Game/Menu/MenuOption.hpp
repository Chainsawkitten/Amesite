#pragma once

#include <glm/glm.hpp>
#include <functional>

class Texture2D;
class Font;
namespace Geometry {
    class Plane;
}
class ShaderProgram;

/// An option in a menu.
class MenuOption {
    public:
        Texture2D* prerenderedText;
        glm::vec3 mPosition;
        glm::vec3 rotation;
        glm::vec2 scale;
        
        std::function<void()> callback;
        
        MenuOption(Font* font, const char* text, const glm::vec3& position, const glm::vec3& rotation, float height);
        ~MenuOption();
        
        glm::mat4 GetModelMatrix() const;
        
        void EmptyCallback() const;
        
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
};
