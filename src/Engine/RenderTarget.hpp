#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader;
class ShaderProgram;

namespace Geometry {
    class Square;
}

/// Render target containing a color and depth buffer.
class RenderTarget {
    public:
        /// Create new render target.
		/**
		 * @param size Size of the new render target.
		 */
		RenderTarget(const glm::vec2& size);
        
        /// Destructor.
        ~RenderTarget();
        
        /// Set as render target.
		void SetTarget();
        
        /// Set as source.
        void SetSource();
        
        /// Get size of the render target.
        /**
         * @return The size of the render target in pixels.
         */
        glm::vec2 GetSize() const;
        
        /// Get color texture.
        /**
         * @return The color buffer's texture.
         */
        GLuint GetColorTexture() const;
        
        /// Get depth texture.
        /**
         * @return The depth buffer's texture.
         */
        GLuint GetDepthTexture() const;
        
        /// Render resulting image to screen.
        void Render();
        
    private:
        unsigned int mWidth;
		unsigned int mHeight;
        
        GLuint mFrameBuffer;
		GLuint mColorBuffer;
		GLuint mDepthBuffer;
        
        Geometry::Square* mSquare;
        
        // Shaders
        Shader* mVertexShader;
        Shader* mFragmentShader;
        ShaderProgram* mShaderProgram;
};
