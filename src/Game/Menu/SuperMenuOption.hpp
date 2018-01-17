#pragma once

#include <glm/glm.hpp>

class Texture2D;
namespace Geometry {
    class Plane;
}
class Shader;
class ShaderProgram;

/// Menu option interface.
class SuperMenuOption {
    public:
        /// Constructor.
        /**
         * @param position Position of the menu option (relative in the menu).
         * @param rotation Rotation of the menu option (relative to the menu).
         */
        SuperMenuOption(const glm::vec3& position, const glm::vec3& rotation);
        
        /// Destructor.
        virtual ~SuperMenuOption();
        
        /// Get the menu option's model matrix.
        /**
         * @return The menu option's model matrix.
         */
        glm::mat4 GetModelMatrix() const;
        
        /// Get the scale of the menu option.
        /**
         * @return Scale of the menu option.
         */
        virtual glm::vec2 GetScale() const = 0;
        
        /// Get prerendered text texture.
        /**
         * @return The prerendered texture.
         */
        virtual Texture2D* GetTexture() const = 0;
        
        /// Press the menu option.
        virtual void Press();
        
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
        Shader* mVertexShader;
        Shader* mTextFragmentShader;
        ShaderProgram* mTextShaderProgram;
        Shader* mSelectedFragmentShader;
        ShaderProgram* mSelectedShaderProgram;
        
        glm::vec3 mPosition;
        glm::vec3 mRotation;
};
