#pragma once

#include <glm/glm.hpp>

class Texture2D;
class ShaderProgram;
namespace Geometry {
    class Plane;
}
class RenderTarget;

/// Reflective water surface.
class Water {
    public:
        /// Create new water surface.
        Water();
        
        /// Destructor.
        ~Water();
        
        /// Update the water.
        /**
         * @param time Time since last frame (in seconds).
         * @param wind The wind velocity.
         */
        void Update(float time, const glm::vec3& wind);
        
        /// Render the reflective surface.
        void Render() const;
        
        /// Get refraction render target.
        /**
         * @return The refraction render target
         */
        RenderTarget* GetRefractionTarget() const;
        
        /// Get reflection render target.
        /**
         * @return The reflection render target
         */
        RenderTarget* GetReflectionTarget() const;
        
        /// Set how much the water textures should repat.
        /**
         * @param textureRepeat How many times the water textures should repeat.
         */
        void SetTextureRepeat(const glm::vec2& textureRepeat);
        
    private:
        ShaderProgram* mShaderProgram;
        
        // Render targets.
        RenderTarget* mRefractionTarget;
        RenderTarget* mReflectionTarget;
        
        // Textures.
        Texture2D* mWaterTexture;
        Texture2D* mDudvMap;
        Texture2D* mNormalMap;
        glm::vec2 mTextureRepeat;
        glm::vec2 mTextureOffset;
        float mMoveFactor;
        
        Geometry::Plane* mPlane;
        
        glm::vec3 mPosition;
        glm::vec3 mScale;
        
        glm::mat4 GetModelMatrix() const;
};
