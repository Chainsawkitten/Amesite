#pragma once

#include <glm/glm.hpp>

class Texture2D;
class ShaderProgram;
namespace Geometry {
    class Plane;
}

/// Reflective water surface.
class Water {
    public:
        /// Create new water surface.
        Water();
        
        /// Destructor.
        ~Water();
        
        /// Set how much the water textures should repat.
        /**
         * @param textureRepeat How many times the water textures should repeat.
         */
        void SetTextureRepeat(const glm::vec2& textureRepeat);
        
    private:
        ShaderProgram* mShaderProgram;
        
        // Textures.
        Texture2D* mWaterTexture;
        Texture2D* mDudvMap;
        Texture2D* mNormalMap;
        glm::vec2 mTextureRepeat;
        
        Geometry::Plane* mPlane;
};
