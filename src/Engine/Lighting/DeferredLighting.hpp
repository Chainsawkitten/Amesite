#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Geometry {
    class Square;
}
class Scene;
class Entity;
class ShaderProgram;
class RenderTarget;

/// Holds the frame buffers used for deferred rendering.
class DeferredLighting {
	public:
		/// The different buffer types.
		enum TEXTURE_TYPE {
			DIFFUSE, ///< Diffuse texture
			NORMAL, ///< Normals
			SPECULAR, ///< Specular
            GLOW, ///< Glow
			NUM_TEXTURES ///< Total number of textures (excluding depth buffer)
		};
        
		/// Create new render target.
		/**
		 * @param size Size of the context.
		 */
		DeferredLighting(const glm::vec2& size);
        
		/// Destructor
		/**
		 * Free allocated resources.
		 */
		~DeferredLighting();
        
        /// Set as render target.
        void SetTarget();
        
        /// Reset render target.
        /**
         * Needs to be called before rendering to the screen.
         */
        void ResetTarget();
        
		/// Render the content of diffuse, position and normal textures.
		/**
		 * @param screenSize Size of the screen in pixels.
		 */
		void ShowTextures(const glm::vec2& screenSize);
        
        /// Render the lighting in the scene.
		/**
         * @param scene Scene to get the lights from.
		 * @param camera Camera to use.
		 * @param renderTarget Target to render to.
		 * @param screenSize Size of the render target in pixels.
		 * @param scale Scaling to apply to texture coordinates.
		 */
		void Render(Scene& scene, Entity* camera, RenderTarget* renderTarget, const glm::vec2& screenSize, float scale = 1.f);
        
	private:
        static void AttachTexture(GLuint texture, unsigned int width, unsigned int height, GLenum attachment, GLint internalFormat);
        void BindForReading();
        void BindForTexReading();
        void SetReadBuffer(TEXTURE_TYPE textureType);
        
		GLuint mTextures[NUM_TEXTURES];
        
		GLuint mFrameBufferObject;
		GLuint mDepthHandle;
        
        GLuint mCelShadingTextures[2];
        GLuint mCelShadingFrameBuffer;
        
		glm::vec2 mSize;
        
        Geometry::Square* mSquare;
        
        ShaderProgram* mShaderProgram;
        ShaderProgram* mCelShadingProgram;
};
