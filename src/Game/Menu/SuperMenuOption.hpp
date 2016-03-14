#pragma once

#include <glm/glm.hpp>

namespace Geometry {
    class Plane;
}
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
        
    private:
        // Used to render 3D text.
        Geometry::Plane* mPlane;
        ShaderProgram* mTextShaderProgram;
        ShaderProgram* mSelectedShaderProgram;
        
        glm::vec3 mPosition;
        glm::vec3 mRotation;
};
